// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/AppConfig/AppConfig.hxx"

using namespace std::chrono_literals;
using namespace std::chrono;

gw::AppConfig::AppConfig() noexcept : autosave_enabled_status_{1}, autosave_interval_{5min}, disk_storage_{InitDiskStorage()} {
    try {
        const auto schema_read = disk_storage_.sync_schema();

        if (schema_read.at("current_settings") == sqlite_orm::sync_schema_result::already_in_sync)
            LoadFromDisk();
        else
            SaveToDisk();
    } catch (...) {
        // TODO: Check for if the Database file is read-only or corrupted
    }
}

auto gw::AppConfig::ToggleAutoSaveStatus() noexcept -> void {
    autosave_enabled_status_ = autosave_enabled_status_ == 0;
    SaveToDisk();
}

// TODO: Optimize atomic operations for speed
[[nodiscard]] auto gw::AppConfig::GetAutoSaveInterval() noexcept -> std::atomic<steady_clock::duration>& { return autosave_interval_; }

// TODO: Optimize atomic operations for speed
auto gw::AppConfig::ChangeAutoSaveInterval(const steady_clock::duration new_interval) noexcept -> void {
    autosave_interval_ = new_interval;
    SaveToDisk();
}

auto gw::AppConfig::GetAutoSaveStatus() const noexcept -> const std::atomic<int>& { return autosave_enabled_status_; }

// TODO: Make this more/stronger type/d safe
auto gw::AppConfig::GetPrintableAutoSaveInterval() const noexcept -> std::string {
    auto ms = duration_cast<milliseconds>(autosave_interval_.load()); // TODO: Optimize for atomic operations speed
    const auto d = duration_cast<days>(ms);
    ms -= d;
    const auto h = duration_cast<hours>(ms);
    ms -= h;
    const auto m = duration_cast<minutes>(ms);
    ms -= m;
    const auto s = duration_cast<seconds>(ms);
    ms -= s;

    std::vector<std::string> bits{};
    if (d.count() > 0) {
        if (d.count() > 1)
            bits.emplace_back(std::format("{} days", d.count()));
        else
            bits.emplace_back(std::format("{} day", d.count()));
    }

    if (h.count() > 0)
        bits.emplace_back(std::format("{} h", h.count()));
    if (m.count() > 0)
        bits.emplace_back(std::format("{} min", m.count()));
    if (s.count() > 0)
        bits.emplace_back(std::format("{} s", s.count()));
    if (ms.count() > 0)
        bits.emplace_back(std::format("{} ms", ms.count()));

    if (bits.empty())
        return "0 s";

    std::string printable_duration{};
    for (std::size_t i{0}; i < bits.size(); ++i) {
        printable_duration += bits[i];

        if (i < bits.size() - 1)
            printable_duration += " : ";
    }

    return printable_duration;
}

// Private Member Methods
auto gw::AppConfig::SaveToDisk() const noexcept -> void {
    try {
        using namespace sqlite_orm;
        using namespace std::chrono;

        const auto status = static_cast<std::int64_t>(autosave_enabled_status_.load());
        const auto interval = static_cast<std::int64_t>(std::chrono::duration_cast<seconds>(autosave_interval_.load()).count());

        const DiskStorageSchema current_settings{.autosave_status = status, .autosave_interval_in_seconds = interval};
        disk_storage_.replace(current_settings);
    } catch (...) {
        // TODO: Find out what is the issue and perform actions...
    }
}

auto gw::AppConfig::LoadFromDisk() noexcept -> void {
    using namespace sqlite_orm;
    using namespace std::chrono;

    try {
        auto results = disk_storage_.get_all<DiskStorageSchema>(where(c(&DiskStorageSchema::id) == 0));

        if (!results.empty()) {
            const auto& read = results.front();
            autosave_enabled_status_ = static_cast<const int>(read.autosave_status);
            autosave_interval_ = seconds(read.autosave_interval_in_seconds);
        }
    } catch (...) {
        // TODO: maybe there are cases to handle..., idk
    }
}

// Private Static Methods
[[nodiscard]] auto gw::AppConfig::InitDiskStorage() noexcept -> DiskStorageType {
    using namespace sqlite_orm;
    return make_storage("settings.db",
                        make_table("current_settings",
                                   make_column("id", &DiskStorageSchema::id, primary_key().autoincrement()),
                                   make_column("autosave_status", &DiskStorageSchema::autosave_status, default_value(1)),
                                   make_column("autosave_interval_in_seconds", &DiskStorageSchema::autosave_interval_in_seconds, default_value(300)),
                                   check(in(&DiskStorageSchema::autosave_status, {0, 1})),
                                   check(c(&DiskStorageSchema::autosave_interval_in_seconds) > 0)));
}