// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "AppSettings.h"

gw::AppSettings::AppSettings(DiskManager& disk_manager) : disk_manager_{disk_manager} {
    const auto result = disk_manager_.LoadSettingsFile();

    if (result.has_value()) {
        auto_save_enabled_status_.store(result->auto_save_enabled_status == true ? 1 : 0, std::memory_order_release);
        auto_save_interval_.store(gw::minutes(result->auto_save_interval_in_minutes), std::memory_order_release);
    }
}

auto gw::AppSettings::RestoreSettingsDefaults() noexcept -> void {
    auto_save_enabled_status_.store(AppSettingsDefaults::auto_save_status_default, std::memory_order_release);
    auto_save_interval_.store(AppSettingsDefaults::auto_save_interval_default, std::memory_order_release);
    disk_manager_.RestoreSettingsDefaults();
}

auto gw::AppSettings::IsAutoSaveEnabled() const noexcept -> bool {
    return auto_save_enabled_status_.load(std::memory_order_acquire) == 1;
}

auto gw::AppSettings::GetAutoSaveInterval() const noexcept -> gw::minutes {
    return auto_save_interval_.load(std::memory_order_acquire);
}

auto gw::AppSettings::ToggleAutoSaveStatus() noexcept -> void {
    assert((auto_save_enabled_status_.load(std::memory_order_acquire) == 1 || auto_save_enabled_status_.load(std::memory_order_acquire) == 0) && "Auto save enabled status different then a 1 or 0");
    auto_save_enabled_status_.fetch_xor(1, std::memory_order_release);
    disk_manager_.ToggleAutoSaveStatus(auto_save_enabled_status_.load(std::memory_order_acquire));
}

auto gw::AppSettings::SetAutoSaveInterval(gw::minutes new_interval) noexcept -> void {
    assert(new_interval.count() >= 0 && "New interval contains negative value");
    auto_save_interval_.store(new_interval, std::memory_order_release);
    disk_manager_.SetAutoSaveInterval(auto_save_interval_.load(std::memory_order_acquire));
}
