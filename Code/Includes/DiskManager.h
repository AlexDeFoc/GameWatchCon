// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <string_view>
#include "GameClockTimeTypes.h"
#include "AppSettingsFile.h"

namespace gw {
class DiskManager {
public:
    /*!
     * @brief Constructs disk manager instance
     * @param console Console ref, used to print error msgs or logs
     */
    DiskManager();
    DiskManager(const DiskManager&) noexcept = delete;
    DiskManager(DiskManager&&) noexcept = delete;
    auto operator=(const DiskManager&) noexcept -> DiskManager& = delete;
    auto operator=(DiskManager&&) noexcept -> DiskManager& = delete;

    // TODO: Add docs
    auto RestoreSettingsDefaults() const noexcept -> void;

    // TODO: Add docs
    auto ResetAllGamesPlaytime() const noexcept -> void;

    // TODO: Add documentation
    auto DeleteAllGames() const noexcept -> void;

    [[nodiscard]] auto LoadSettingsFile() const noexcept -> std::optional<SettingsFile>;

    auto LoadGamesLibraryFile() const noexcept -> void; // TODO: Void temporarly, same with nodiscard abscense

    auto AddNewGame(std::string&&) const noexcept -> void;

    /*!
     * @brief Toggles auto save status in settings table in storage
     */
    auto ToggleAutoSaveStatus(bool new_value) const noexcept -> void;

    /*!
     * @brief Sets auto save interval in settings table in storage
     * @param new_interval Interval in seconds
     */
    auto SetAutoSaveInterval(gw::minutes new_interval) const noexcept -> void;

    /*!
     * @brief Sets game title for a given game id in games table in storage
     * @param game_id
     * @param game_title
     */
    auto SetGameTitle(int, std::string&&) const noexcept -> void;

    // TODO: Remake other doxygen comments like this one! Simple and clear
    /*!
     * @brief Resets game playtime of a given game id
     * @param game_id
     */
    auto ResetGamePlaytime(int) const noexcept -> void;

    /*!
     * @brief Deletes game based on given game id
     * @param game_id
     */
    auto DeleteGame(int) const noexcept -> void;

    auto AddGamePlaytime(int, long long) const noexcept -> void;

private:
    [[nodiscard]] static auto GetExeDirPath() noexcept -> std::string;
    [[nodiscard]] static auto GetSettingsFilepath() noexcept -> std::string;
    [[nodiscard]] static auto GetGamesLibraryFilepath() noexcept -> std::string;
    [[nodiscard]] static auto GetSettingsBackupFilepath() noexcept -> std::string;
    [[nodiscard]] static auto GetGamesLibraryBackupFilepath() noexcept -> std::string;

    mutable std::mutex mutex_;
    static constexpr std::string_view settings_file_name_{"settings.json"};
    static constexpr std::string_view games_library_file_name_{"games_library.json"};
    static constexpr std::string_view settings_backup_file_name_{"settings.bak.json"};
    static constexpr std::string_view games_library_backup_file_name_{"games_library.bak.json"};
    const std::string settings_file_path_;
    const std::string games_library_file_path_;
    const std::string settings_backup_file_path_;
    const std::string games_library_backup_file_path_;
};
} // namespace gw
