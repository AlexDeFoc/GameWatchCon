// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <atomic>

namespace gw {
class AppState {
public:
    AppState() noexcept = default;
    AppState(const AppState&) noexcept = delete;
    AppState(AppState&&) noexcept = delete;
    auto operator=(const AppState&) noexcept -> AppState& = delete;
    auto operator=(AppState&&) noexcept -> AppState& = delete;
    [[nodiscard]] auto IsAppStillRunning() const noexcept -> bool;
    auto ToggleAppRunningState() noexcept -> void;

private:
    std::atomic<std::int8_t> keep_app_running_{1};
};
} // namespace gw
