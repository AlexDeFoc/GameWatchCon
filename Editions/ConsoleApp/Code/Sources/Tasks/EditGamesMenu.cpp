// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "Console.h"
#include "AppSettings.h"
#include "GameLibrary.h"
#include "Tasks/EditGamesMenu.h"
#include "Tasks/MainMenu.h"
#include "Tasks/ChangeGameTitle.h"
#include "Tasks/ResetGamePlaytime.h"
#include "Tasks/DeleteGame.h"

auto gw::tasks::EditGamesMenu(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::Tag::Info, "No games found which to edit!");
        return Task{gw::tasks::MainMenu};
    }

    // TODO: Check the compiler warning
    auto menu_opts = [&] -> const std::array<std::string, 4> {
        return {
            {"Change game title",
             "Reset game clock",
             "Delete game",
             "Go back"}};
    };

    const auto [opt_sel, input_status] = console.RequestUserMenuChoice(menu_opts(), false);

    switch (input_status) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    switch (opt_sel) {
        case 0:
            return Task{gw::tasks::MainMenu};

        case 1:
            return Task{gw::tasks::ChangeGameTitle};

        case 2:
            return Task{gw::tasks::ResetGamePlaytime};

        case 3:
            return Task{gw::tasks::DeleteGame};

        default:
            console.ThrowOnUnhandledCase();
    }
}
