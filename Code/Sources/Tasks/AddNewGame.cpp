// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/AddNewGame.h"
#include "Tasks/MainMenu.h"

auto gw::tasks::AddNewGame(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    auto [new_game_title, input_status] = console.RequestGameTitle();

    switch (input_status) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return Task{gw::tasks::MainMenu};

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    game_library.AddNewGame(std::move(new_game_title));

    console.WriteLineToCache(Console::Tag::Success, "Added new game");

    return Task{gw::tasks::MainMenu};
}
