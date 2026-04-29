// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "Tasks/StartApp.h"
#include "Tasks/MainMenu.h"

auto gw::tasks::StartApp(gw::Console&, gw::AppState&, gw::AppSettings&, gw::GameLibrary&) -> Task {
    return Task{gw::tasks::MainMenu};
}
