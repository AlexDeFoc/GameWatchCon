// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "TaskDispatcher.h"
#include "Tasks/StartApp.h"

gw::TaskDispatcher::TaskDispatcher() : next_task_{gw::tasks::StartApp}, app_state_{}, console_{}, disk_manager_{}, app_settings_{disk_manager_}, game_library_{disk_manager_, app_settings_} {}

auto gw::TaskDispatcher::Start() -> void {
    while (app_state_.IsAppStillRunning() && next_task_.func != nullptr) {
        next_task_ = next_task_.func(console_, app_state_, app_settings_, game_library_);
    }
}
