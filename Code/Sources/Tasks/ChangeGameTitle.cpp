// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Console.hpp"
#include "GameLibrary.hpp"
#include "Tasks/ChangeGameTitle.hpp"
#include "Tasks/EditGamesMenu.hpp"

auto gw::tasks::ChangeGameTitle(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    assert(game_library.GetGameCount() != 0 && "Attemted to game title while no games exist");

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

    auto [new_game_title, input_status_for_game_title] = console.RequestGameTitle();

    switch (input_status_for_game_title) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return Task{gw::tasks::EditGamesMenu};

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    game_library.SetGameTitle(selected_game_id, std::move(new_game_title));

    // TODO: Add more context
    console.WriteLineToCache(Console::Tag::Success, "Changed game title");

    return Task{gw::tasks::EditGamesMenu};
}

