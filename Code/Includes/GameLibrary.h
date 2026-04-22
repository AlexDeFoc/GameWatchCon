// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <vector>
#include "AppSettings.h"

namespace gw {
class GameLibrary {
public:
    // TODO: Add doxygen comments for all methods in the app
    GameLibrary(DiskManager&, AppSettings&);
    [[nodiscard]] auto IsAnyGameActive() const noexcept -> bool;
    [[nodiscard]] auto ActiveGameTitle() const noexcept -> std::string;
    [[nodiscard]] auto IsEmpty() const noexcept -> bool;
    [[nodiscard]] auto GetGameCount() const noexcept -> std::size_t;
    [[nodiscard]] auto GetActiveGameId() const noexcept -> std::size_t;
    [[nodiscard]] auto GetAllGames() const noexcept -> std::vector<gw::DiskManager::GameInStorage>;
    auto AddNewGame(std::string&&) const noexcept -> void;
    auto SetGameTitle(std::size_t, std::string&&) const noexcept -> void;
    auto ResetGamePlaytime(std::size_t) const noexcept -> void;
    auto DeleteGame(std::size_t) const noexcept -> void;

private:
    AppSettings& app_settings_;
    DiskManager& disk_manager_;

    std::atomic<int> should_save_game{0};
    std::atomic<bool> took_snapshot_already_{false};
    std::atomic<std::size_t> active_game_id_{0};
    std::chrono::steady_clock::time_point last_snaphot{};
};
} // namespace gw