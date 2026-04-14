// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "pch.hxx"

namespace gw {
class AppConfig {
public:
    AppConfig() noexcept;

    auto ToggleAutoSaveStatus() noexcept -> void;
    auto ChangeAutoSaveInterval(std::chrono::steady_clock::duration) noexcept -> void;
    [[nodiscard]] auto IsAutoSaveEnabled() const noexcept -> bool;
    [[nodiscard]] auto GetAutoSaveIntervalInMilliseconds() const noexcept -> std::chrono::milliseconds;
    [[nodiscard]] auto GetAutoSaveInterval() const noexcept -> std::chrono::steady_clock::duration;
    [[nodiscard]] auto GetPrintableAutoSaveInterval() const noexcept -> std::string;

private:
    struct DiskStorageSchema {
        std::int64_t id = 0;
        int64_t autosave_status = 0;
        int64_t autosave_interval_in_seconds = 0;
    };

    using DiskStorageType = decltype(sqlite_orm::make_storage("settings.db",
                                                              sqlite_orm::make_table("current_settings",
                                                                                     sqlite_orm::make_column("id", &DiskStorageSchema::id, sqlite_orm::primary_key().autoincrement()),
                                                                                     sqlite_orm::make_column("autosave_status", &DiskStorageSchema::autosave_status, sqlite_orm::default_value(1)),
                                                                                     sqlite_orm::make_column("autosave_interval_in_seconds", &DiskStorageSchema::autosave_interval_in_seconds, sqlite_orm::default_value(300)),
                                                                                     sqlite_orm::check(sqlite_orm::in(&DiskStorageSchema::autosave_status, {0, 1})),
                                                                                     sqlite_orm::check(sqlite_orm::c(&DiskStorageSchema::autosave_interval_in_seconds) > 0))));

    bool autosave_enabled_status_ = true;
    std::chrono::steady_clock::duration autosave_interval_ = std::chrono::minutes(5);
    mutable DiskStorageType disk_storage_;
    mutable std::mutex mutex_;

    // Private Member Methods
    auto SaveToDisk() const noexcept -> void;
    auto LoadFromDisk() noexcept -> void;

    // Private Static Methods
    [[nodiscard]] static auto InitDiskStorage() noexcept -> DiskStorageType;
};
} // namespace gw