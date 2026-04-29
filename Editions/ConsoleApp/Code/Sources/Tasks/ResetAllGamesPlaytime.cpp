// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/ResetAllGamesPlaytime.h"
#include "Tasks/SettingsMenu.h"

auto gw::tasks::ResetAllGamesPlaytime(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::Tag::Info, "No games found which to reset playtime!");
        return Task{gw::tasks::SettingsMenu};
    }

    const auto [user_is_sure, input_status_for_user_confirmation] = console.RequestUserConfirmation();

    switch (input_status_for_user_confirmation) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    if (user_is_sure) {
        game_library.ResetAllGamesPlaytime();
        console.WriteLineToCache(Console::Tag::Success, "Reset all games playtime");
    } else
        console.WriteLineToCache(Console::Tag::Info, "Action cancelled");

    return Task{gw::tasks::SettingsMenu};
}
