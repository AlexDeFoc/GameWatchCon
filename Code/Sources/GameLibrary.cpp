// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "GameLibrary.h"

gw::GameLibrary::GameLibrary(Console& console, DiskManager& disk_manager, AppSettings& app_settings) : console_{console}, app_settings_{app_settings}, disk_manager_{disk_manager} {
    auto result = disk_manager_.LoadGamesLibraryFile();

    if (result.has_value())
        games_ = result.value();

    autosave_thread_ = std::jthread([this](const std::stop_token& stop_token) { SaveJob(stop_token); });
}

gw::GameLibrary::~GameLibrary() {
    // 1. Signal auto thread to stop
    autosave_thread_.request_stop();

    // 2. Make autosave thread wake up
    should_save_game_.store(2, std::memory_order_release); // NOTE: We use 2 to signal 'Shutdown' and not interact with the logic of 1 and 0

    // 3. Notify the thread
    should_save_game_.notify_all();

    if (app_settings_.IsAutoSaveEnabled())
        autosave_cv_.notify_all();
}

auto gw::GameLibrary::IsAnyGameActive() const noexcept -> bool {
    bool auto_save_enabled = app_settings_.IsAutoSaveEnabled();

    if (auto_save_enabled)
        return should_save_game_.load(std::memory_order_relaxed) == 1;
    else {
        bool took = took_snapshot_already_.load(std::memory_order_relaxed);
        bool pending = should_save_game_.load(std::memory_order_relaxed) == 1;

        return pending ? !took : took;
    }
}

auto gw::GameLibrary::ActiveGameTitle() const noexcept -> std::string_view {
    assert(active_game_id_ - 1 >= 0 && "Active game id was <= 0");
    return games_[static_cast<std::size_t>(active_game_id_.load(std::memory_order_acquire) - 1)].GetTitle(); // TODO: Check if we can relax the memory order
}

auto gw::GameLibrary::IsEmpty() const noexcept -> bool {
    return games_.empty();
}

auto gw::GameLibrary::GetGameCount() const noexcept -> std::int64_t {
    return static_cast<std::int64_t>(games_.size());
}

auto gw::GameLibrary::GetActiveGameId() const noexcept -> std::int64_t {
    return active_game_id_.load(std::memory_order_acquire);
}

auto gw::GameLibrary::AddNewGame(std::string&& game_title) noexcept -> void {
    games_.emplace_back(std::move(game_title));
    disk_manager_.UpdateGamesLibraryFile(games_);
}

auto gw::GameLibrary::SetGameTitle(const std::int64_t game_id, std::string&& game_title) noexcept -> void {
    assert(game_id - 1 >= 0 && "Game id <= 0");
    games_[static_cast<std::size_t>(game_id - 1)].SetGameTitle(std::move(game_title));
    disk_manager_.UpdateGamesLibraryFile(games_);
}

auto gw::GameLibrary::ResetGamePlaytime(const std::int64_t game_id) noexcept -> void {
    assert(game_id - 1 >= 0 && "Game id <= 0");
    games_[static_cast<std::size_t>(game_id - 1)].ResetPlaytime();
    disk_manager_.UpdateGamesLibraryFile(games_);
}

auto gw::GameLibrary::DeleteGame(const std::int64_t game_id) noexcept -> void {
    assert(game_id - 1 >= 0 && "Game id <= 0");
    games_.erase(games_.begin() + game_id - 1);
    disk_manager_.UpdateGamesLibraryFile(games_);
}

auto gw::GameLibrary::ToggleGameClock(const std::int64_t game_id) noexcept -> void {
    active_game_id_.store(game_id, std::memory_order_release);

    if (!app_settings_.IsAutoSaveEnabled()) {
        should_save_game_.store(1, std::memory_order_release);
    } else {
        should_save_game_.fetch_xor(1);
    }

    should_save_game_.notify_all();
    autosave_cv_.notify_all();
}

auto gw::GameLibrary::AddGameTime(const std::chrono::steady_clock::duration time) noexcept -> void {
    using namespace std::chrono;
    games_[static_cast<std::size_t>(active_game_id_.load(std::memory_order_acquire) - 1)].AddPlaytime(duration_cast<gw::seconds>(time));
    disk_manager_.UpdateGamesLibraryFile(games_);
}

auto gw::GameLibrary::SaveJob(const std::stop_token& stop_token) noexcept -> void {
    while (!stop_token.stop_requested()) {
        should_save_game_.wait(0, std::memory_order_acquire);

        if (stop_token.stop_requested())
            break;

        if (!app_settings_.IsAutoSaveEnabled()) {
            if (!took_snapshot_already_.load(std::memory_order_relaxed)) {
                last_snapshot_ = std::chrono::steady_clock::now();
                took_snapshot_already_.store(true, std::memory_order_release);
            } else {
                AddGameTime(std::chrono::steady_clock::now() - last_snapshot_);
                took_snapshot_already_.store(false, std::memory_order_release);
            }

            should_save_game_.store(0, std::memory_order_release);
        } else {
            last_snapshot_ = std::chrono::steady_clock::now();

            // LOOP until game is toggled OFF or the app closes
            while (!stop_token.stop_requested() && should_save_game_.load(std::memory_order_acquire) == 1) {
                std::unique_lock<std::mutex> lck{mutex_};

                bool interrupted = autosave_cv_.wait_for(lck, stop_token, app_settings_.GetAutoSaveInterval(), [&] {
                    return should_save_game_.load(std::memory_order_acquire) == 0;
                });

                auto now = std::chrono::steady_clock::now();
                AddGameTime(now - last_snapshot_);
                last_snapshot_ = now;

                if (stop_token.stop_requested() || interrupted)
                    break;
            }
        }
    }
}

auto gw::GameLibrary::GetPrintableGames(const Console& console) noexcept -> std::string {
    // clang-format off
    const auto is_any_game_active = IsAnyGameActive();
    const auto active_game_id = GetActiveGameId();

    return games_ | std::views::enumerate | std::views::transform([&](auto&& pair) {
        auto&& [game_index, game] = pair;

        using namespace std::chrono;
        using namespace std::string_view_literals;

        auto s = game.GetPlaytime();
        const auto d = duration_cast<days>(s); s -= d;
        const auto h = duration_cast<hours>(s); s -= h;
        const auto m = duration_cast<minutes>(s); s -= m;

        auto values = std::array<std::int64_t, 4>{d.count(), h.count(), m.count(), s.count()};
        auto labels = std::array<std::string_view, 4>{"day"sv, "h"sv, "min"sv, "s"sv};

        auto duration_str = std::views::zip(values, labels)
                          | std::views::filter([](auto&& tuple){ return std::get<0>(tuple) > 0; })
                          | std::views::transform([](auto&& tuple) {
                              auto [val, label] = tuple;
                              return std::format("{} {}{}", val, label, (label == "day" && val > 1 ? "s" : ""));
                          })
                          | std::views::join_with(" : "sv)
                          | std::ranges::to<std::string>();

        const bool is_this_game_active = is_any_game_active && (game_index + 1 == active_game_id);
        const std::string active_tag = is_this_game_active ? std::format(" - {}", console.ColorText(Console::Color::Red, "ACTIVE")) : "";

        return std::format("{}. {} - {}{}", game_index + 1, game.GetTitle(), (duration_str.empty() ? "0 s" : duration_str), active_tag);
    })           | std::views::join_with('\n')
                 | std::ranges::to<std::string>();
    // clang-format on
}

auto gw::GameLibrary::DeleteAllGames() noexcept -> void {
    games_.clear();
    disk_manager_.UpdateGamesLibraryFile(games_);
}

auto gw::GameLibrary::ResetAllGamesPlaytime() noexcept -> void {
    for (auto& game : games_)
        game.ResetPlaytime();
    disk_manager_.UpdateGamesLibraryFile(games_);
}

auto gw::GameLibrary::CreateGamesDatabaseBackup() const noexcept -> void {
    const auto err = disk_manager_.CreateGamesLibraryFileBackup();

    using ResType = gw::DiskManager::CreateGamesLibraryFileBackupResult;

    if (err) {
        switch (*err) {
            case ResType::GamesLibraryFileNotFound:
                console_.WriteLineToCache(Console::Tag::Error, "Failed to create games library backup, file doesn't exist!");
                break;

            case ResType::UnknownError:
                console_.WriteLineToCache(Console::Tag::FatalError, "Failed to create games library backup, unknown filesystem error occured!");
                break;

            default:
                console_.ThrowOnUnhandledCase();
        }
    } else {
        console_.WriteLineToCache(Console::Tag::Success, "Created games library backup");
    }
}
