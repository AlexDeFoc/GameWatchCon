// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Tasks/EditGamesMenu/EditGamesMenu.hxx"

auto gw::tasks::EditGamesMenu(Console& console, const GameLibrary& game_library) noexcept -> TaskType {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::MsgType::Error, "No entries found");
        return TaskType::MainMenu;
    }

    static auto list_opts = [] {
        std::println("1. Change game title");
        std::println("2. Reset game clock");
        std::println("3. Delete game");
        std::println("0. Go back");
    };

    [&] {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();
            console.RequestMenuOptionID(list_opts, {0, 3}, Console::RequestIsCancellable::No);

            switch (console.GetInputRequestStatus()) {
                case Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(Console::MsgType::Error, "Invalid input!");
                    break;

                case Console::InputRequestStatus::Success:
                    return;

                default:
                    assert(false && "Unhandled Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    switch (console.GetNumberInputResult()) {

        case 0:
            return TaskType::MainMenu;

        case 1:
            return TaskType::ChangeGameTitle;

        case 2:
            return TaskType::ResetGameClock;

        case 3:
            return TaskType::DeleteGame;

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}