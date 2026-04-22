// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <atomic>

namespace gw {
class AppState {
public:
    AppState() noexcept = default;
    [[nodiscard]] auto IsAppStillRunning() const noexcept -> bool;
    auto ToggleAppRunningState() noexcept -> void;

private:
    std::atomic<int> keep_app_running_{1};
};
} // namespace gw