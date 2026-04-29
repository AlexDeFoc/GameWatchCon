// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <vector>
#include "AppSettingsDefaults.h"
#include "Console.h"
#include "GameEntry.h"

namespace gw {
class DiskManager {
public:
    /*!
     * @brief Constructs disk manager instance
     * @param console Console ref, used to print error msgs or logs
     */
    DiskManager(Console&);
    DiskManager(const DiskManager&) noexcept = delete;
    DiskManager(DiskManager&&) noexcept = delete;
    auto operator=(const DiskManager&) noexcept -> DiskManager& = delete;
    auto operator=(DiskManager&&) noexcept -> DiskManager& = delete;

    // TODO: Add docs
    auto CreateGamesDatabaseBackup() noexcept -> void;

    // TODO: Add docs
    auto RestoreSettingsDefaults() const noexcept -> void;

    // TODO: Add docs
    auto ResetAllGamesPlaytime() const noexcept -> void;

    // TODO: Add documentation
    auto DeleteAllGames() const noexcept -> void;

    /*!
     * @brief Get auto save enabled status from storage
     * @return auto save enabled status value
     */
    [[nodiscard]] auto GetAutoSaveEnabledStatusValue() noexcept -> long long;

    /*!
     * @brief Returns auto save interval from storage
     * @return Auto save interval value
     */
    [[nodiscard]] auto GetAutoSaveIntervalValue() const noexcept -> long long;

    /*!
     * @brief Get game title from storage
     * @param game_id
     * @return game title
     */
    //[[nodiscard]] auto GetGameTitle(int) const noexcept -> std::string;

    /*!
     * @brief Gets game count from games table from storage
     * @return Game count
     */
    [[nodiscard]] auto GameCount() const noexcept -> int;

    /*!
     * @brief Adds new game into games table in storage
     * @param game_title Title for the game
     */
    auto AddNewGame(std::string&&) const noexcept -> void;

    /*!
     * @brief Toggles auto save status in settings table in storage
     */
    auto ToggleAutoSaveStatus(bool new_value) const noexcept -> void;

    /*!
     * @brief Sets auto save interval in settings table in storage
     * @param new_interval Interval in seconds
     */
    auto SetAutoSaveIntervalValue(long long) const noexcept -> void;

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
    struct SettingsFile {
        bool auto_save_enabled_status{AppSettingsDefaults::auto_save_status_default};
        std::int64_t auto_save_interval_in_minutes{AppSettingsDefaults::auto_save_interval_default.count()};
    };

    struct GamesLibraryFile {
        std::vector<GameEntryBase>& games;
    };

    const std::string settings_file_path_;

    static constexpr std::string_view settings_file_name_{"settings.json"};
    [[nodiscard]] auto GetExeDirPath() const noexcept -> std::string;
    [[nodiscard]] auto GetSettingsFilepath() noexcept -> std::string;
    [[nodiscard]] auto GetStorageBackupPath() noexcept -> std::string;
    [[nodiscard]] auto GetStorageTemporaryBackupPath() noexcept -> std::string;
    [[nodiscard]] auto GetDropDbColumnsConfirmationFilePath() const noexcept -> std::string;
    // auto InitStorage() -> std::unique_ptr<StorageType>;
    auto BackupTemporarlyStorageFile() -> void;
    auto RestoreTempBackupStorageFile() noexcept -> void;
    auto DeleteTempBackupStorageFile() noexcept -> void;
    [[nodiscard]] auto UserIsSureToDropColumns() const noexcept -> bool;

    static constexpr std::string_view storage_backup_filename_{"data.bak.json"};
    static constexpr std::string_view storage_temp_backup_filename_{"data.bak.tmp.json"};
    static constexpr std::string_view drop_db_columns_confirmation_file_filename{"user_is_sure_to_drop_db_columns_on_next_app_startup.txt"};
    Console& console_;
    const std::string storage_backup_path_;
    static constexpr int storage_ver_{1};
    mutable std::mutex mutex_;
};
} // namespace gw
