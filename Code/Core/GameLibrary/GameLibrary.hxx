// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/AppConfig/AppConfig.hxx"
#include "Core/AppState/AppState.hxx"
#include "Core/Console/Console.hxx"
#include "Core/GameEntry/GameEntry.hxx"

namespace gw {
class GameLibrary {
public:
    explicit GameLibrary(const AppState&, const AppConfig&) noexcept;
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

    [[nodiscard]] auto GetActiveGameIndex() const noexcept -> std::size_t;

    [[nodiscard]] auto GamesCount() const noexcept -> std::size_t;

    [[nodiscard]] auto IsAnyGameActive() const noexcept -> bool;

private:
    struct DiskGameSchema {
        std::int64_t id = 0;
        std::string title;
        std::int64_t clock_in_ms = 0;
    };

    using DiskStorageType = decltype(sqlite_orm::make_storage("game_library.db",
                                                              sqlite_orm::make_table("games",
                                                                                     sqlite_orm::make_column("id", &DiskGameSchema::id, sqlite_orm::primary_key().autoincrement()),
                                                                                     sqlite_orm::make_column("title", &DiskGameSchema::title, sqlite_orm::collate_binary()),
                                                                                     sqlite_orm::make_column("clock_in_ms", &DiskGameSchema::clock_in_ms, sqlite_orm::default_value(0)),
                                                                                     sqlite_orm::check(sqlite_orm::c(&DiskGameSchema::clock_in_ms) >= 0))));

    std::atomic<std::size_t> active_game_index_ = 0;
    std::chrono::steady_clock::time_point last_snapshot;
    std::atomic<bool> took_snapshot_already_ = false;
    std::vector<GameEntry> games_;
    mutable DiskStorageType disk_storage_;

    mutable std::mutex mutex_;
    std::jthread autosave_thread_;
    std::condition_variable autosave_cv_;

    std::atomic<std::uint8_t> should_save_game_ = 0;
    const AppState& app_state_;
    const AppConfig& app_config_;

    // Private Member Methods
    auto SaveJob() noexcept -> void;
    auto SaveToDisk() const noexcept -> void;
    auto LoadFromDisk() noexcept -> void;

    // Private Static Methods
    [[nodiscard]] static auto InitDiskStorage() noexcept -> DiskStorageType;
};
} // namespace gw