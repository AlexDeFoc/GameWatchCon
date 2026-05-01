// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <vector>
#include <condition_variable>
#include <thread>
#include <string_view>
#include "AppSettings.h"
#include "GameEntry.h"
#include "Console.h"

namespace gw {
class GameLibrary {
public:
    // TODO: Add doxygen comments for all methods in the app
    GameLibrary(Console&, DiskManager&, AppSettings&);
    GameLibrary(const GameLibrary&) noexcept = delete;
    GameLibrary(GameLibrary&&) noexcept = delete;
    auto operator=(const GameLibrary&) noexcept -> GameLibrary& = delete;
    auto operator=(GameLibrary&&) noexcept -> GameLibrary& = delete;
    ~GameLibrary();
    [[nodiscard]] auto IsAnyGameActive() const noexcept -> bool;
    [[nodiscard]] auto ActiveGameTitle() const noexcept -> std::string_view;
    [[nodiscard]] auto IsEmpty() const noexcept -> bool;
    [[nodiscard]] auto GetGameCount() const noexcept -> std::int64_t;
    [[nodiscard]] auto GetActiveGameId() const noexcept -> std::int64_t;
    auto AddNewGame(std::string&& game_title) noexcept -> void;
    auto SetGameTitle(std::int64_t, std::string&&) noexcept -> void;
    auto ResetGamePlaytime(std::int64_t) noexcept -> void;
    auto DeleteGame(std::int64_t) noexcept -> void;
    auto ToggleGameClock(std::int64_t) noexcept -> void;
    auto GetPrintableGames(const Console&) noexcept -> std::string;
    auto DeleteAllGames() noexcept -> void;
    auto ResetAllGamesPlaytime() noexcept -> void;
    auto CreateGamesDatabaseBackup() const noexcept -> void;

private:
    auto AddGameTime(std::chrono::steady_clock::duration) noexcept -> void;
    auto SaveJob(const std::stop_token&) noexcept -> void;

    Console& console_;
    AppSettings& app_settings_;
    DiskManager& disk_manager_;

    std::atomic<std::int8_t> should_save_game_{0};
    std::atomic<bool> took_snapshot_already_{false};
    std::atomic<std::int64_t> active_game_id_{0};
    std::chrono::steady_clock::time_point last_snaphot{};
    std::condition_variable_any autosave_cv_;
    std::jthread autosave_thread_;
    std::chrono::steady_clock::time_point last_snapshot_;
    mutable std::mutex mutex_;
    std::vector<GameEntry> games_;
};
} // namespace gw
