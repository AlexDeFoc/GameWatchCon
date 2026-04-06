/*
    GameWatchCon - Keep track of your in-game time
    Copyright (C) 2026  Sava Alexandru-Andrei

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

module;

#include <chrono>

module GameClock;

using namespace gw::con::core;

GameClock::GameClock() noexcept : duration_{std::chrono::steady_clock::duration::zero()} {}

auto GameClock::AddTime(const std::chrono::steady_clock::duration additional_time) noexcept -> void { duration_ += additional_time; }

auto GameClock::Reset() noexcept -> void { duration_ = std::chrono::steady_clock::duration::zero(); }

auto GameClock::GetDays() const noexcept -> std::chrono::days { return std::chrono::duration_cast<std::chrono::days>(duration_); }

auto GameClock::GetHours() const noexcept -> std::chrono::hours {
    const auto total_days = std::chrono::duration_cast<std::chrono::days>(duration_);
    const auto total_hours = std::chrono::duration_cast<std::chrono::hours>(duration_);
    return total_hours - total_days;
}

auto GameClock::GetMinutes() const noexcept -> std::chrono::minutes {
    const auto total_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration_);
    const auto total_hours = std::chrono::duration_cast<std::chrono::hours>(duration_);
    return total_minutes - total_hours;
}

auto GameClock::GetSeconds() const noexcept -> std::chrono::seconds {
    const auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_);
    const auto total_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration_);
    return total_seconds - total_minutes;
}

auto GameClock::GetMilliseconds() const noexcept -> std::chrono::milliseconds {
    const auto total_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_);
    const auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_);
    return total_milliseconds - total_seconds;
}