// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Core/GameEntry/GameEntry.hxx"

gw::GameEntry::GameEntry() noexcept = default;

gw::GameEntry::GameEntry(std::string title) noexcept : title_{std::move(title)} {}

gw::GameEntry::GameEntry(std::string title, const std::chrono::steady_clock::duration time) noexcept : title_{std::move(title)}, clock_{time} {}

auto gw::GameEntry::SetTitle(std::string new_title) noexcept -> void { title_ = std::move(new_title); }

auto gw::GameEntry::GetTitle() const noexcept -> std::string_view { return title_; }

auto gw::GameEntry::AddTime(std::chrono::steady_clock::duration additional_time) noexcept -> void { clock_.AddTime(std::move(additional_time)); }

auto gw::GameEntry::ResetClock() noexcept -> void { clock_.Reset(); }

auto gw::GameEntry::GetPrintableClock() const noexcept -> std::string {
    std::vector<std::string> clock_bits{};

    if (clock_.GetDays() > std::chrono::days::zero()) {
        if (clock_.GetDays() > std::chrono::days(1))
            clock_bits.emplace_back(std::format("{} days", clock_.GetDays().count()));
        else
            clock_bits.emplace_back(std::format("{} day", clock_.GetDays().count()));
    }

    if (clock_.GetHours() > std::chrono::hours::zero())
        clock_bits.emplace_back(std::format("{} h", clock_.GetHours().count()));
    if (clock_.GetMinutes() > std::chrono::minutes::zero())
        clock_bits.emplace_back(std::format("{} min", clock_.GetMinutes().count()));
    if (clock_.GetSeconds() > std::chrono::seconds::zero())
        clock_bits.emplace_back(std::format("{} s", clock_.GetSeconds().count()));
    if (clock_.GetMilliseconds() > std::chrono::milliseconds::zero())
        clock_bits.emplace_back(std::format("{} ms", clock_.GetMilliseconds().count()));

    if (clock_bits.empty())
        return "0 s";

    std::string printable_clock{};
    for (std::size_t i{0}; i < clock_bits.size(); ++i) {
        printable_clock += clock_bits[i];

        if (i < clock_bits.size() - 1)
            printable_clock += " : ";
    }

    return printable_clock;
}

auto gw::GameEntry::GetTitleForStoringToDisk() const noexcept -> std::string { return title_; }

auto gw::GameEntry::GetClockForStoringToDisk() const noexcept -> std::int64_t { return clock_.GetTimeForStoringToDisk(); }