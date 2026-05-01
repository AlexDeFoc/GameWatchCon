// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Tasks/StopApp.hpp"
#include "AppState.hpp"

auto gw::tasks::StopApp(gw::Console&, gw::AppState& app_state, gw::AppSettings&, gw::GameLibrary&) -> Task {
    app_state.ToggleAppRunningState();
    return Task{};
}

