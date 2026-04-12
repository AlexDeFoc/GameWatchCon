// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/Console/Console.hxx"
#include "Core/GameEntry/GameEntry.hxx"

namespace gw {
class GameLibrary {
public:
    explicit GameLibrary(std::atomic<int>&, std::atomic<std::chrono::steady_clock::duration>&, const std::atomic<int>&) noexcept;
    ~GameLibrary();

    auto SetGameTitle(std::size_t, std::string) noexcept -> void;

    auto SetGameTitle(std::size_t, std::string_view) noexcept -> void;

    auto ResetGameClock(std::size_t) noexcept -> void;

    auto RemoveGame(std::size_t) noexcept -> void;

    auto AddGame(std::string_view) noexcept -> void;

    auto AddGame(std::string) noexcept -> void;

    auto AddGameTime(std::chrono::steady_clock::duration) noexcept -> void;

    auto ToggleGameClock(std::optional<int>) noexcept -> void;

    auto ListGames(const Console&) const noexcept -> void;

    [[nodiscard]] auto IsEmpty() const noexcept -> bool;

    [[nodiscard]] auto GetGameTitle(std::size_t) const noexcept -> std::string_view;

    [[nodiscard]] auto GetActiveGameTitle() const noexcept -> std::string_view;

    [[nodiscard]] auto GetActiveGameIndex() const noexcept -> int;

    [[nodiscard]] auto GamesCount() const noexcept -> std::size_t;

    [[nodiscard]] auto IsAnyGameActive() const noexcept -> bool;

private:
    int active_game_index_ = 0;
    std::chrono::steady_clock::time_point last_snapshot;
    bool took_snapshot_already_ = false;
    std::vector<GameEntry> games_;

    std::mutex mutex_;
    std::jthread autosave_thread_;
    std::condition_variable autosave_cv_;

    bool should_save_game_ = false;
    std::atomic<int>& keep_thread_running_;

    const std::atomic<int>& autosave_enabled_status;
    std::atomic<std::chrono::steady_clock::duration>& autosave_interval_;

    auto SaveJob() noexcept -> void;
};
} // namespace gw