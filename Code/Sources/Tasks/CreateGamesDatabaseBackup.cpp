// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Console.hpp"
#include "GameLibrary.hpp"
#include "Tasks/CreateGamesDatabaseBackup.hpp"
#include "Tasks/SettingsMenu.hpp"

auto gw::tasks::CreateGamesDatabaseBackup(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::Tag::Info, "No games found which to backup!");
        return Task{gw::tasks::SettingsMenu};
    }

    game_library.CreateGamesDatabaseBackup();
    return Task{gw::tasks::SettingsMenu};
}

