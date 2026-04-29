// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/MainMenu.h"
#include "Tasks/StopApp.h"
#include "Tasks/ListGames.h"
#include "Tasks/EditGamesMenu.h"
#include "Tasks/AddNewGame.h"
#include "Tasks/SettingsMenu.h"
#include "Tasks/CheckForUpdates.h"
#include "Tasks/ToggleGameClock.h"

auto gw::tasks::MainMenu(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    std::array<std::string, 7> menu_opts = {"List games",
                                            "Start game",
                                            "Edit games",
                                            "Add new game",
                                            "Settings",
                                            "Check for updates",
                                            "Exit App"};

    if (game_library.IsAnyGameActive())
        menu_opts[1] = std::format("Stop game: {}", game_library.ActiveGameTitle());
    else
        menu_opts[1] = "Start game";

    const auto [opt_sel, input_status] = console.RequestUserMenuChoice(menu_opts, false);

    switch (input_status) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    switch (opt_sel) {
        case 0:
            return Task{gw::tasks::StopApp};

        case 1:
            return Task{gw::tasks::ListGames};

        case 2:
            return Task{gw::tasks::ToggleGameClock};

        case 3:
            return Task{gw::tasks::EditGamesMenu};

        case 4:
            return Task{gw::tasks::AddNewGame};

        case 5:
            return Task{gw::tasks::SettingsMenu};

        case 6:
            return Task{gw::tasks::CheckForUpdates};

        default:
            console.ThrowOnUnhandledCase();
    }
}
