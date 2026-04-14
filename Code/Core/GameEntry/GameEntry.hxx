// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/GameClock/GameClock.hxx"

namespace gw {
// TODO: Optimize its size, its too much!
class GameEntry {
public:
    GameEntry() noexcept;
    explicit GameEntry(std::string) noexcept;
    explicit GameEntry(std::string, std::chrono::steady_clock::duration) noexcept;

    auto SetTitle(std::string) noexcept -> void;
    [[nodiscard]] auto GetTitle() const noexcept -> std::string_view;
    auto AddTime(std::chrono::steady_clock::duration) noexcept -> void;
    auto ResetClock() noexcept -> void;
    [[nodiscard]] auto GetPrintableClock() const noexcept -> std::string;

    [[nodiscard]] auto GetTitleForStoringToDisk() const noexcept -> std::string;
    [[nodiscard]] auto GetClockForStoringToDisk() const noexcept -> std::int64_t;

private:
    std::string title_;
    GameClock clock_;
};
} // namespace gw