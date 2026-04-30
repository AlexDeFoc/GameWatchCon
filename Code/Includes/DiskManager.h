// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <string_view>
#include <vector>
#include "GameClockTimeTypes.h"
#include "AppSettingsFile.h"
#include "GameEntry.h"

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

    auto RestoreSettingsDefaults() const noexcept -> void;

    [[nodiscard]] auto LoadSettingsFile() const noexcept -> std::optional<SettingsFile>;

    [[nodiscard]] auto LoadGamesLibraryFile() const noexcept -> std::optional<std::vector<GameEntry>>;

    auto UpdateGamesLibraryFile(std::vector<GameEntry>& local_games_library) const noexcept -> void;

    /*!
     * @brief Toggles auto save status in settings table in storage
     */
    auto ToggleAutoSaveStatus(bool new_value) const noexcept -> void;

    /*!
     * @brief Sets auto save interval in settings table in storage
     * @param new_interval Interval in seconds
     */
    auto SetAutoSaveInterval(gw::minutes new_interval) const noexcept -> void;

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
