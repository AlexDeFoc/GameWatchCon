// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/StopApp/StopApp.hxx"

auto gw::tasks::StopApp(std::atomic<int>& keep_app_running_status) noexcept -> TaskType {
    keep_app_running_status = 0; // TODO: Optimize for speed
    return {};
}