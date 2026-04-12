// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/AddNewGame/AddNewGame.hxx"

auto gw::tasks::AddNewGame(Console& console, GameLibrary& game_library) noexcept -> TaskType {
    console.ClearScreen();
    console.RequestGameTitle();

    switch (console.GetInputRequestStatus()) {
        case Console::InputRequestStatus::Success:
            game_library.AddGame(console.GetStringInputResult());
            console.WriteLineToCache(Console::MsgType::Info, "Added game");
            return TaskType::MainMenu;

        case Console::InputRequestStatus::Cancelled:
            console.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
            return TaskType::MainMenu;

        case Console::InputRequestStatus::Invalid:
            console.WriteLineToCache(Console::MsgType::Info, "An issue has occurred with adding the game");
            return TaskType::MainMenu;

        default:
            break;
    }

    assert(false && "Unhandled Console::InputRequestStatus in AddNewGame::Run");
    std::terminate();
}