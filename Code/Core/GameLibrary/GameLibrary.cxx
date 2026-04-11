// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/GameLibrary/GameLibrary.hxx"

auto gw::GameLibrary::SaveJob() noexcept -> void {
    std::unique_lock lock{mutex_};

    do {
        // Sleep while predicate is false; gets skipped (predicate gets checked) when using notify_one()
        autosave_cv_.wait(lock, [&] { return should_save_game_ || !keep_thread_running_; });

        // Autosave status table:
        // 0 => do manual save
        // 1 => perform autosave
        if (autosave_enabled_status == 0) {
            if (!took_snapshot_already_) {
                last_snapshot = std::chrono::steady_clock::now();
                took_snapshot_already_ = true;
            } else {
                AddGameTime(std::chrono::steady_clock::now() - last_snapshot);
                took_snapshot_already_ = false;
            }

            should_save_game_ = false; // App will re-enable each time to either start or stop
        } else {
            last_snapshot = std::chrono::steady_clock::now();

            // Sleep until we elapsed autosave interval OR we should stop before finishing the autosave interval OR app is exiting
            autosave_cv_.wait_for(lock, autosave_interval_.load(), [&] { return !should_save_game_ || !keep_thread_running_; });

            AddGameTime(std::chrono::steady_clock::now() - last_snapshot);
        }
    } while (keep_thread_running_);
}

gw::GameLibrary::GameLibrary(std::atomic<int>& keep_app_running_status, std::atomic<std::chrono::steady_clock::duration>& autosave_interval, const std::atomic<int>& autosave_status) noexcept : keep_thread_running_{keep_app_running_status}, autosave_enabled_status{autosave_status}, autosave_interval_{autosave_interval} {
    autosave_thread_ = std::jthread([&] { SaveJob(); });
}

gw::GameLibrary::~GameLibrary() {
    // 1. App sets app state to off

    // 2. Notify the thread to read the app state
    autosave_cv_.notify_one();

    // 3. Join the thread
    if (autosave_thread_.joinable())
        autosave_thread_.join();
}

auto gw::GameLibrary::SetGameTitle(const std::size_t index, std::string new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].SetTitle(std::move(new_title));
}

auto gw::GameLibrary::SetGameTitle(const std::size_t index, const std::string_view new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].SetTitle(std::string{new_title});
}

auto gw::GameLibrary::ResetGameClock(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].ResetClock();
}

auto gw::GameLibrary::RemoveGame(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_.erase(games_.begin() + index);
}

auto gw::GameLibrary::AddGame(const std::string_view title) noexcept -> void { games_.emplace_back(std::string{title}); }

auto gw::GameLibrary::AddGameTime(const std::chrono::steady_clock::duration time) noexcept -> void { games_[active_game_index_].AddTime(time); }

auto gw::GameLibrary::AddGame(std::string title) noexcept -> void { games_.emplace_back(std::move(title)); }

auto gw::GameLibrary::ToggleGameClock() noexcept -> void {
    // Case 1: autosave_enabled_status == 0 => we set it to true each time
    // Case 2: autosave_enabled_status == 1 => we set it to true first then false the second time we toggle
    if (autosave_enabled_status == 0)
        should_save_game_ = true;
    else
        should_save_game_ = should_save_game_ == false;

    autosave_cv_.notify_one();
}

auto gw::GameLibrary::ListGames() const noexcept -> void {
    for (std::size_t game_index{1}; const auto& game : games_) {
        std::println("{}. {} - {}", game_index, game.GetTitle(), game.GetPrintableClock());
        game_index++;
    }
}

auto gw::GameLibrary::IsEmpty() const noexcept -> bool { return games_.empty(); }

auto gw::GameLibrary::GetGameTitle(const std::size_t index) const noexcept -> std::string_view {
    assert(index < games_.size() && "Provided out of range index to GameLibraryReadAccess::GetGameTitle");
    return games_[index].GetTitle();
}

auto gw::GameLibrary::GamesCount() const noexcept -> std::size_t { return games_.size(); }