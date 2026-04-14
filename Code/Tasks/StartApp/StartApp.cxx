// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Tasks/StartApp/StartApp.hxx"

auto gw::tasks::StartApp() noexcept -> TaskType {
    return TaskType::MainMenu;
}