// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Tasks/ListGames.hpp"
#include "Tasks/MainMenu.hpp"
#include "Console.hpp"
#include "GameLibrary.hpp"

auto gw::tasks::ListGames(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::Tag::Info, "No games found which to list!");
        return Task{gw::tasks::MainMenu};
    }

    console.ClearCout();

    std::println("{}", game_library.GetPrintableGames(console));

    console.WriteLine(Console::Tag::Tip, "Press any key to go back");
    Console::RequestKeyPress();

    return Task{gw::tasks::MainMenu};
}
