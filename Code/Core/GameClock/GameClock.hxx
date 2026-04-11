// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "pch.hxx"

namespace gw {
class GameClock {
public:
    GameClock() noexcept;

    auto AddTime(std::chrono::steady_clock::duration) noexcept -> void;
    auto Reset() noexcept -> void;
    [[nodiscard]] auto GetDays() const noexcept -> std::chrono::days;
    [[nodiscard]] auto GetHours() const noexcept -> std::chrono::hours;
    [[nodiscard]] auto GetMinutes() const noexcept -> std::chrono::minutes;
    [[nodiscard]] auto GetSeconds() const noexcept -> std::chrono::seconds;
    [[nodiscard]] auto GetMilliseconds() const noexcept -> std::chrono::milliseconds;

private:
    std::chrono::steady_clock::duration duration_;
};
} // namespace gw