// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "Console.h"
#include "AppSettings.h"
#include "GameLibrary.h"
#include "Tasks/ToggleAutoSaveStatus.h"
#include "Tasks/SettingsMenu.h"

auto gw::tasks::ToggleAutoSaveStatus(gw::Console& console, gw::AppState&, gw::AppSettings& app_settings, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsAnyGameActive()) {
        console.WriteLineToCache(Console::Tag::Error, "Cannot toggle auto save status while a game is active. Stop it first!");
        return Task{gw::tasks::SettingsMenu};
    }

    app_settings.ToggleAutoSaveStatus();
    return Task{gw::tasks::SettingsMenu};
}
