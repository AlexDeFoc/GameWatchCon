// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/AppConfig/AppConfig.hxx"

using namespace std::chrono_literals;
using namespace std::chrono;

// TODO: change autosave to being true by default
gw::AppConfig::AppConfig() noexcept : autosave_enabled_status_{false}, autosave_interval_{5min} {}

auto gw::AppConfig::ToggleAutoSaveStatus() noexcept -> void { autosave_enabled_status_ = autosave_enabled_status_ == AutoSaveStatus::Disabled ? AutoSaveStatus::Enabled : AutoSaveStatus::Disabled; }

auto gw::AppConfig::ChangeAutoSaveInterval(const steady_clock::duration new_interval) noexcept -> void { autosave_interval_ = new_interval; }

auto gw::AppConfig::GetAutoSaveStatus() const noexcept -> AutoSaveStatus { return autosave_enabled_status_; }

// TODO: Make this more/stronger type/d safe
auto gw::AppConfig::GetPrintableAutoSaveInterval() const noexcept -> std::string {
    auto ms = duration_cast<milliseconds>(autosave_interval_);
    const auto d = duration_cast<days>(ms);
    ms -= d;
    const auto h = duration_cast<hours>(ms);
    ms -= h;
    const auto m = duration_cast<minutes>(ms);
    ms -= m;
    const auto s = duration_cast<seconds>(ms);
    ms -= s;

    std::vector<std::string> bits{};
    if (d.count() > 0)
        bits.emplace_back(std::format("{} days", d.count()));
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