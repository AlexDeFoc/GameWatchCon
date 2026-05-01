// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "AppState.hpp"

[[nodiscard]] auto gw::AppState::IsAppStillRunning() const noexcept -> bool {
    return keep_app_running_.load(std::memory_order_acquire) == 1;
}

auto gw::AppState::ToggleAppRunningState() noexcept -> void {
    keep_app_running_.fetch_xor(1, std::memory_order_release);
}

