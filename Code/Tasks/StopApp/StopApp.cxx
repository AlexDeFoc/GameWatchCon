// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Tasks/StopApp/StopApp.hxx"

auto gw::tasks::StopApp(AppState& app_state) noexcept -> TaskType {
    app_state.ToggleAppRunningStatus();
    return TaskType::MainMenu;
}