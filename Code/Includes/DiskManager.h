// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <memory>
#include <vector>
#include <sqlite_orm/sqlite_orm.h>
#include "AppSettingsDefaults.h"
#include "Console.h"

namespace gw {
class DiskManager {
public:
    struct GameInStorage {
        std::int64_t id{};
        std::string title{};
        std::int64_t playtime_in_ms{};
    };

    /*!
     * @brief Constructs disk manager instance
     * @param console Console ref, used to print error msgs or logs
     */
    DiskManager(Console&);

    /*!
     * @brief Get auto save enabled status from storage
     * @return auto save enabled status value
     */
    [[nodiscard]] auto GetAutoSaveEnabledStatusValue() noexcept -> std::size_t;

    /*!
    * @brief Returns auto save interval from storage
    * @return Auto save interval value
    */
    [[nodiscard]] auto GetAutoSaveIntervalValue() const noexcept -> std::size_t;

    /*!
     * @brief Get game title from storage
     * @param game_id
     * @return game title
     */
    [[nodiscard]] auto GetGameTitle(std::size_t) const noexcept -> std::string;

    /*!
    * @brief Gets game count from games table from storage
    * @return Game count
    */
    [[nodiscard]] auto GameCount() const noexcept -> std::size_t;

    /*!
    * @brief Gets all games from games table from storage
    * @return Vector of games
    */
    [[nodiscard]] auto GetAllGames() const noexcept -> std::vector<GameInStorage>;

    /*!
    * @brief Adds new game into games table in storage
    * @param game_title Title for the game
    */
    auto AddNewGame(std::string&&) const noexcept -> void;

    /*!
    * @brief Toggles auto save status in settings table in storage
    */
    auto ToggleAutoSaveStatus() const noexcept -> void;

    /*!
    * @brief Sets auto save interval in settings table in storage
    * @param new_interval Interval in seconds
    */
    auto SetAutoSaveIntervalValue(std::size_t) const noexcept -> void;

    /*!
    * @brief Sets game title for a given game id in games table in storage
    * @param game_id
    * @param game_title
    */
    auto SetGameTitle(std::size_t, std::string&&) const noexcept -> void;

    // TODO: Remake other doxygen comments like this one! Simple and clear
    /*!
    * @brief Resets game playtime of a given game id
    * @param game_id
    */
    auto ResetGamePlaytime(std::size_t) const noexcept -> void;

    /*!
     * @brief Deletes game based on given game id
     * @param game_id
     */
    auto DeleteGame(std::size_t) const noexcept -> void;

private:
    struct SettingsInStorage {
        std::int64_t id{1};
        std::int64_t auto_save_enabled_status{AppSettingsDefaults::auto_save_status_default};
        std::int64_t auto_save_interval_in_sec{std::chrono::duration_cast<std::chrono::seconds>(AppSettingsDefaults::auto_save_interval_default).count()};
    };

    // clang-format off
    const std::string storage_path_;

    using StorageType = decltype(sqlite_orm::make_storage(storage_path_, sqlite_orm::make_table("settings", sqlite_orm::make_column("setting_id", &SettingsInStorage::id, sqlite_orm::primary_key()),
                                                                                                            sqlite_orm::make_column("auto_save_enabled_status", &SettingsInStorage::auto_save_enabled_status, sqlite_orm::default_value(AppSettingsDefaults::auto_save_status_default), sqlite_orm::check(sqlite_orm::in(&SettingsInStorage::auto_save_enabled_status, {0, 1}))),
                                                                                                            sqlite_orm::make_column("auto_save_interval_in_sec", &SettingsInStorage::auto_save_interval_in_sec, sqlite_orm::default_value(std::chrono::duration_cast<std::chrono::seconds>(AppSettingsDefaults::auto_save_interval_default).count()), sqlite_orm::check(sqlite_orm::c(&SettingsInStorage::auto_save_interval_in_sec) >= 0))),
                                                                         sqlite_orm::make_table("games", sqlite_orm::make_column("id", &GameInStorage::id, sqlite_orm::primary_key()),
                                                                                                         sqlite_orm::make_column("title", &GameInStorage::title),
                                                                                                         sqlite_orm::make_column("playtime_in_ms", &GameInStorage::playtime_in_ms, sqlite_orm::default_value(0), sqlite_orm::check(sqlite_orm::c(&GameInStorage::playtime_in_ms) >= 0)))));
    // clang-format on

    static constexpr std::string_view storage_filename_{"data.db"};
    [[nodiscard]] auto GetExeDirPath() const noexcept -> std::string;
    [[nodiscard]] auto GetStoragePath() noexcept -> std::string;
    [[nodiscard]] auto GetStorageBackupPath() noexcept -> std::string;
    [[nodiscard]] auto GetStorageTemporaryBackupPath() noexcept -> std::string;
    [[nodiscard]] auto GetDropDbColumnsConfirmationFilePath() const noexcept -> std::string;
    auto InitStorage() -> std::unique_ptr<StorageType>;
    auto BackupTemporarlyStorageFile() -> void;
    auto RestoreTempBackupStorageFile() noexcept -> void;
    auto DeleteTempBackupStorageFile() noexcept -> void;
    [[nodiscard]] auto UserIsSureToDropColumns() const noexcept -> bool;

    static constexpr std::string_view storage_backup_filename_{"data.bak.db"};
    static constexpr std::string_view storage_temp_backup_filename_{"data.bak.tmp.db"};
    static constexpr std::string_view drop_db_columns_confirmation_file_filename{"user_is_sure_to_drop_db_columns_on_next_app_startup.txt"};
    Console& console_;
    const std::string storage_backup_path_;
    static constexpr int storage_ver_{1};
    std::unique_ptr<StorageType> storage_;
};
} // namespace gw