// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/GameClock/GameClock.hxx"

gw::GameClock::GameClock() noexcept : duration_{std::chrono::steady_clock::duration::zero()} {}

auto gw::GameClock::AddTime(const std::chrono::steady_clock::duration additional_time) noexcept -> void { duration_ += additional_time; }

auto gw::GameClock::Reset() noexcept -> void { duration_ = std::chrono::steady_clock::duration::zero(); }

auto gw::GameClock::GetDays() const noexcept -> std::chrono::days { return std::chrono::duration_cast<std::chrono::days>(duration_); }

auto gw::GameClock::GetHours() const noexcept -> std::chrono::hours {
    const auto total_days = std::chrono::duration_cast<std::chrono::days>(duration_);
    const auto total_hours = std::chrono::duration_cast<std::chrono::hours>(duration_);
    return total_hours - total_days;
}

auto gw::GameClock::GetMinutes() const noexcept -> std::chrono::minutes {
    const auto total_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration_);
    const auto total_hours = std::chrono::duration_cast<std::chrono::hours>(duration_);
    return total_minutes - total_hours;
}

auto gw::GameClock::GetSeconds() const noexcept -> std::chrono::seconds {
    const auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_);
    const auto total_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration_);
    return total_seconds - total_minutes;
}

auto gw::GameClock::GetMilliseconds() const noexcept -> std::chrono::milliseconds {
    const auto total_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_);
    const auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_);
    return total_milliseconds - total_seconds;
}