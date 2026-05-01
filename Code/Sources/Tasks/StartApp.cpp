// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Tasks/StartApp.hpp"
#include "Tasks/MainMenu.hpp"

auto gw::tasks::StartApp(gw::Console&, gw::AppState&, gw::AppSettings&, gw::GameLibrary&) -> Task {
    return Task{gw::tasks::MainMenu};
}

