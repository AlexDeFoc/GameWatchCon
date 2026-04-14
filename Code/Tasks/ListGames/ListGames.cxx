// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Tasks/ListGames/ListGames.hxx"

auto gw::tasks::ListGames(Console& console, const GameLibrary& game_library) noexcept -> TaskType {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::MsgType::Error, "No entries found");
        return TaskType::MainMenu;
    }

    console.ClearScreen();
    game_library.ListGames(console);
    console.WriteLine(Console::MsgType::Tip, "Enter anything to go back");
    Console::RequestKeyPress();
    return TaskType::MainMenu;
}