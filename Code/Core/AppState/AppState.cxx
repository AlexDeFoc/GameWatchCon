// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Core/AppState/AppState.hxx"

auto gw::AppState::IsAppRunning() const noexcept -> bool {
    std::lock_guard lck{mutex_};
    return app_running_status_;
}

auto gw::AppState::ToggleAppRunningStatus() noexcept -> void {
    std::lock_guard lck{mutex_};
    app_running_status_ = app_running_status_ == false ? true : false;
}