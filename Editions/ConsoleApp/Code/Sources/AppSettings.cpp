// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "AppSettings.h"

gw::AppSettings::AppSettings(DiskManager& disk_manager) : disk_manager_{disk_manager} {}

auto gw::AppSettings::RestoreSettingsDefaults() const noexcept -> void {
    disk_manager_.RestoreSettingsDefaults();
}

auto gw::AppSettings::IsAutoSaveEnabled() const noexcept -> bool {
    return disk_manager_.GetAutoSaveEnabledStatusValue() != 0;
}

auto gw::AppSettings::GetAutoSaveInterval() const noexcept -> std::chrono::minutes {
    return std::chrono::minutes(disk_manager_.GetAutoSaveIntervalValue());
}

auto gw::AppSettings::ToggleAutoSaveStatus() const noexcept -> void {
    disk_manager_.ToggleAutoSaveStatus();
}

auto gw::AppSettings::SetAutoSaveInterval(std::chrono::minutes new_interval) const noexcept -> void {
    disk_manager_.SetAutoSaveIntervalValue(new_interval.count());
}