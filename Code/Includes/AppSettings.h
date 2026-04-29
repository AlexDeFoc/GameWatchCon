// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "GameClockTimeTypes.h"
#include "DiskManager.h"

namespace gw {
class AppSettings {
public:
    /*!
     * @brief Contructs app settings instance
     * @param disk_manager DiskManager ref, used to perform the actions of the this class
     */
    AppSettings(DiskManager&);
    AppSettings(const AppSettings&) noexcept = delete;
    AppSettings(AppSettings&&) noexcept = delete;
    auto operator=(const AppSettings&) noexcept -> AppSettings& = delete;
    auto operator=(AppSettings&&) noexcept -> AppSettings& = delete;

    // TODO: Add docs
    auto RestoreSettingsDefaults() noexcept -> void;

    /*!
     * @brief Returns whether auto save is enabled or not (loads status from disk)
     * @return Auto save enabled status
     */
    [[nodiscard]] auto IsAutoSaveEnabled() const noexcept -> bool;

    /*!
     * @brief Returns the auto save interval (loads interval from disk)
     * @return Auto save interval
     */
    [[nodiscard]] auto GetAutoSaveInterval() const noexcept -> gw::minutes;

    /*!
     * @brief Toggles auto save status
     */
    auto ToggleAutoSaveStatus() noexcept -> void;

    /*!
     * @brief Changes auto save interval
     * @param new_interval Interval in seconds
     */
    auto SetAutoSaveInterval(gw::minutes) noexcept -> void;

private:
    [[maybe_unused]] DiskManager& disk_manager_; // TODO: Remove the nodiscard attribute
    std::atomic<std::int8_t> auto_save_enabled_status_{AppSettingsDefaults::auto_save_status_default};
    std::atomic<gw::minutes> auto_save_interval_{AppSettingsDefaults::auto_save_interval_default};
};
} // namespace gw
