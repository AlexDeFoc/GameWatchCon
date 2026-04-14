// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "pch.hxx"

namespace gw {
class AppState {
public:
    AppState() noexcept = default;
    [[nodiscard]] auto IsAppRunning() const noexcept -> bool;
    auto ToggleAppRunningStatus() noexcept -> void;

private:
    mutable std::mutex mutex_;
    bool app_running_status_ = true;
};
} // namespace gw