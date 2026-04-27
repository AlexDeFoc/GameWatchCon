// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <vector>
#include <condition_variable>
#include <thread>
#include "AppSettings.h"

namespace gw {
class GameLibrary {
public:
    // TODO: Add doxygen comments for all methods in the app
    GameLibrary(DiskManager&, AppSettings&);
    GameLibrary(const GameLibrary&) noexcept = delete;
    GameLibrary(GameLibrary&&) noexcept = delete;
    auto operator=(const GameLibrary&) noexcept -> GameLibrary& = delete;
    auto operator=(GameLibrary&&) noexcept -> GameLibrary& = delete;
    ~GameLibrary();
    [[nodiscard]] auto IsAnyGameActive() const noexcept -> bool;
    [[nodiscard]] auto ActiveGameTitle() const noexcept -> std::string;
    [[nodiscard]] auto IsEmpty() const noexcept -> bool;
    [[nodiscard]] auto GetGameCount() const noexcept -> int;
    [[nodiscard]] auto GetActiveGameId() const noexcept -> int;
    [[nodiscard]] auto GetAllGames() const noexcept -> std::vector<gw::DiskManager::GameInStorage>;
    auto AddNewGame(std::string&&) const noexcept -> void;
    auto SetGameTitle(int, std::string&&) const noexcept -> void;
    auto ResetGamePlaytime(int) const noexcept -> void;
    auto DeleteGame(int) const noexcept -> void;
    auto ToggleGameClock(int) noexcept -> void;
    auto GetPrintableGames(const Console&) const noexcept -> std::string;
    auto DeleteAllGames() const noexcept -> void;
    auto ResetAllGamesPlaytime() const noexcept -> void;
    auto CreateGamesDatabaseBackup() const noexcept -> void;

private:
    auto AddGameTime(std::chrono::steady_clock::duration) const noexcept -> void;
    auto SaveJob(std::stop_token) noexcept -> void;

    AppSettings& app_settings_;
    DiskManager& disk_manager_;

    std::atomic<int> should_save_game_{0};
    std::atomic<bool> took_snapshot_already_{false};
    std::atomic<int> active_game_id_{0};
    std::chrono::steady_clock::time_point last_snaphot{};
    std::condition_variable_any autosave_cv_;
    std::jthread autosave_thread_;
    std::chrono::steady_clock::time_point last_snapshot_;
    mutable std::mutex mutex_;
};
} // namespace gw