// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/StartApp/StartApp.hxx"

auto gw::tasks::StartApp(bool& keep_app_running_status) noexcept -> TaskType {
    keep_app_running_status = true;
    return TaskType::MainMenu;
}