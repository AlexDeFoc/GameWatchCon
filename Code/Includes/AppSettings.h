// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "DiskManager.h"

namespace gw {
class AppSettings {
public:
    /*!
     * @brief Contructs app settings instance
     * @param disk_manager DiskManager ref, used to perform the actions of the this class
     */
    AppSettings(DiskManager&);

    /*!
     * @brief Returns whether auto save is enabled or not (loads status from disk)
     * @return Auto save enabled status
     */
    [[nodiscard]] auto IsAutoSaveEnabled() const noexcept -> bool;

    /*!
     * @brief Returns the auto save interval (loads interval from disk)
     * @return Auto save interval
     */
    [[nodiscard]] auto GetAutoSaveInterval() const noexcept -> std::chrono::seconds;

    /*!
    * @brief Toggles auto save status
    */
    [[nodiscard]] auto ToggleAutoSaveStatus() const noexcept -> void;

    /*!
    * @brief Changes auto save interval
    * @param new_interval Interval in seconds
    */
    auto SetAutoSaveInterval(std::chrono::seconds) const noexcept -> void;

private:
    DiskManager& disk_manager_;
};
} // namespace gw