// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Core/GameLibrary/GameLibrary.hxx"
#include "Core/Utils/Utils.hxx"

gw::GameLibrary::GameLibrary(const AppState& app_state, const AppConfig& app_config) noexcept : disk_storage_{InitDiskStorage()}, app_state_{app_state}, app_config_{app_config} {
    autosave_thread_ = std::jthread([this](std::stop_token stop_token) { SaveJob(stop_token); });

    try {
        const auto schema_read = disk_storage_.sync_schema();

        if (schema_read.at("games") == sqlite_orm::sync_schema_result::already_in_sync)
            LoadFromDisk();
        else
            SaveToDisk();
    } catch (...) {
        // TODO: Check for if the Database file is read-only or corrupted
    }
}

gw::GameLibrary::~GameLibrary() {
    // 1. Signal auto thread to stop
    autosave_thread_.request_stop();

    // 2. Make autosave thread wake up
    should_save_game_.store(2, std::memory_order_release); // NOTE: We use 2 to signal 'Shutdown' and not interact with the logic of 1 and 0

    // 3. Notify the thread
    should_save_game_.notify_all();

    if (app_config_.IsAutoSaveEnabled())
        autosave_cv_.notify_all();
}

auto gw::GameLibrary::SetGameTitle(const std::size_t index, std::string new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    auto disk_game = disk_storage_.get<DiskGameSchema>(index + 1);
    disk_game.title = new_title;
    disk_storage_.update(disk_game);
    games_[index].SetTitle(std::move(new_title));
}

auto gw::GameLibrary::SetGameTitle(const std::size_t index, const std::string_view new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].SetTitle(std::string{new_title});
    auto disk_game = disk_storage_.get<DiskGameSchema>(index + 1);
    disk_game.title = std::string{new_title};
    disk_storage_.update(disk_game);
}

auto gw::GameLibrary::ResetGameClock(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].ResetClock();
    auto disk_game = disk_storage_.get<DiskGameSchema>(index + 1);
    disk_game.clock_in_ms = 0;
    disk_storage_.update(disk_game);
}

auto gw::GameLibrary::RemoveGame(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_.erase(games_.begin() + index);
    disk_storage_.remove<DiskGameSchema>(index + 1);
}

auto gw::GameLibrary::AddGame(const std::string_view title) noexcept -> void {
    games_.emplace_back(std::string{title});
    disk_storage_.insert(DiskGameSchema{.title = std::string{title}});
}

auto gw::GameLibrary::AddGame(std::string title) noexcept -> void {
    disk_storage_.insert(DiskGameSchema{.title = title});
    games_.emplace_back(std::move(title));
}

auto gw::GameLibrary::AddGameTime(const std::chrono::steady_clock::duration time) noexcept -> void {
    auto current_active_game_index = active_game_index_.load(std::memory_order_acquire);

    games_[current_active_game_index].AddTime(time);
    auto disk_game = disk_storage_.get<DiskGameSchema>(current_active_game_index + 1);
    disk_game.clock_in_ms += std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    disk_storage_.update(disk_game);
}

auto gw::GameLibrary::ToggleGameClock(const std::optional<int> index) noexcept -> void {
    if (index != std::nullopt)
        active_game_index_.store(*index, std::memory_order_release);

    // Case 1: autosave_enabled_status == 0 => we set it to true each time
    // Case 2: autosave_enabled_status == 1 => we set it to true first then false the second time we toggle
    if (!app_config_.IsAutoSaveEnabled())
        should_save_game_.store(1, std::memory_order_release);
    else {
        should_save_game_.fetch_xor(1); // Invert status each time
    }

    should_save_game_.notify_all();
    autosave_cv_.notify_all();
}

// TODO: Maybe transition from in-memory games to disk-only games
auto gw::GameLibrary::ListGames(const Console& console) const noexcept -> void {
    for (std::size_t game_index{1}; const auto& game : games_) {
        if (IsAnyGameActive() && game_index == active_game_index_.load(std::memory_order_relaxed) + 1)
            std::println("{}. {} - {} - {}", game_index, game.GetTitle(), game.GetPrintableClock(), utils::ColorText(console, utils::TextColor::Red, "ACTIVE"));
        else
            std::println("{}. {} - {}", game_index, game.GetTitle(), game.GetPrintableClock());
        game_index++;
    }
}

auto gw::GameLibrary::IsEmpty() const noexcept -> bool { return games_.empty(); }

auto gw::GameLibrary::GetGameTitle(const std::size_t index) const noexcept -> std::string_view {
    assert(index < games_.size() && "Provided out of range index to GameLibraryReadAccess::GetGameTitle");
    return games_[index].GetTitle();
}

auto gw::GameLibrary::GetActiveGameTitle() const noexcept -> std::string_view {
    return games_[active_game_index_.load(std::memory_order_acquire)].GetTitle();
}

auto gw::GameLibrary::GetActiveGameIndex() const noexcept -> std::size_t { return active_game_index_.load(std::memory_order_acquire); }

auto gw::GameLibrary::GamesCount() const noexcept -> std::size_t { return games_.size(); }

auto gw::GameLibrary::IsAnyGameActive() const noexcept -> bool {
    bool auto_save_enabled = app_config_.IsAutoSaveEnabled();

    if (auto_save_enabled)
        return should_save_game_.load(std::memory_order_relaxed);
    else {
        bool took = took_snapshot_already_.load(std::memory_order_relaxed);
        bool pending = should_save_game_.load(std::memory_order_relaxed) == 1;

        return pending ? !took : took;
    }
}

auto gw::GameLibrary::SaveJob(std::stop_token stop_token) noexcept -> void {
    while (!stop_token.stop_requested()) {
        should_save_game_.wait(0, std::memory_order_acquire);

        if (stop_token.stop_requested())
            break;

        if (!app_config_.IsAutoSaveEnabled()) {
            if (!took_snapshot_already_.load(std::memory_order_relaxed)) {
                last_snapshot = std::chrono::steady_clock::now();
                took_snapshot_already_.store(true, std::memory_order_release);
            } else {
                AddGameTime(std::chrono::steady_clock::now() - last_snapshot);
                took_snapshot_already_.store(false, std::memory_order_release);
            }

            should_save_game_.store(0, std::memory_order_release);
        } else {
            last_snapshot = std::chrono::steady_clock::now();

            // LOOP until game is toggled OFF or the app closes
            while (!stop_token.stop_requested() && should_save_game_.load(std::memory_order_acquire) == 1) {
                std::unique_lock lck{mutex_};

                bool interrupted = autosave_cv_.wait_for(lck, stop_token, app_config_.GetAutoSaveInterval(), [&] {
                    return should_save_game_.load(std::memory_order_acquire) == 0;
                });

                auto now = std::chrono::steady_clock::now();
                AddGameTime(now - last_snapshot);
                last_snapshot = now;

                if (stop_token.stop_requested() || interrupted)
                    break;
            }
        }
    }
}

auto gw::GameLibrary::SaveToDisk() const noexcept -> void {
    try {
        using namespace sqlite_orm;

        auto guard = disk_storage_.transaction_guard();

        for (std::int64_t i{0}; static_cast<std::size_t>(i) < games_.size(); ++i) {
            const DiskGameSchema disk_game{.id = i, .title = games_[i].GetTitleForStoringToDisk(), .clock_in_ms = games_[i].GetClockForStoringToDisk()};
            disk_storage_.replace(disk_game);
        }

        guard.commit();
    } catch (...) {
        // TODO: Find out what is the issue and perform actions...
    }
}

auto gw::GameLibrary::LoadFromDisk() noexcept -> void {
    using namespace sqlite_orm;
    using namespace std::chrono;

    try {
        auto results = disk_storage_.get_all<DiskGameSchema>();

        if (!results.empty()) {
            for (const auto& disk_game : results)
                games_.emplace_back(disk_game.title, milliseconds(disk_game.clock_in_ms));
        }
    } catch (...) {
        // TODO: maybe there are cases to handle..., idk
    }
}

// Private Static Methods
auto gw::GameLibrary::InitDiskStorage() noexcept -> DiskStorageType {
    using namespace sqlite_orm;
    return make_storage("game_library.db",
                        make_table("games",
                                   make_column("id", &DiskGameSchema::id, primary_key().autoincrement()),
                                   make_column("title", &DiskGameSchema::title, collate_binary()),
                                   make_column("clock_in_ms", &DiskGameSchema::clock_in_ms, default_value(0)),
                                   check(c(&DiskGameSchema::clock_in_ms) >= 0)));
}