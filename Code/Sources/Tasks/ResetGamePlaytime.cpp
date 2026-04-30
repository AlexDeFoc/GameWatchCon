// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/ResetGamePlaytime.h"
#include "Tasks/EditGamesMenu.h"

auto gw::tasks::ResetGamePlaytime(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    assert(game_library.GetGameCount() != 0 && "Attemted to reset game playtime while no games exist");

    const auto [selected_game_id, input_status_for_game_id] = console.RequestUserGameIDChoice(game_library.GetPrintableGames(console), game_library.GetGameCount());

    switch (input_status_for_game_id) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return Task{gw::tasks::EditGamesMenu};

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    const auto [user_is_sure, input_status_for_user_confirmation] = console.RequestUserConfirmation();

    switch (input_status_for_user_confirmation) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    if (user_is_sure) {
        game_library.ResetGamePlaytime(selected_game_id);
        console.WriteLineToCache(Console::Tag::Success, "Reset game playtime"); // TODO: Add more context
    } else
        console.WriteLineToCache(Console::Tag::Info, "Action cancelled");

    return Task{gw::tasks::EditGamesMenu};
}
