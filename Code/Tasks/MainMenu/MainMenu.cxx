// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/MainMenu/MainMenu.hxx"

auto gw::tasks::MainMenu(Console& console) noexcept -> TaskType {
    static auto list_opts = [] {
        std::println("1. List games");
        std::println("2. Start game");
        std::println("3. Edit games");
        std::println("4. Add new game");
        std::println("5. Settings");
        std::println("6. Check for updates");
        std::println("0. Exit app");
    };

    [&] {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();
            console.RequestMenuOptionID(list_opts, {0, 6}, Console::RequestIsCancellable::No);

            switch (console.GetInputRequestStatus()) {
                case Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(Console::MsgType::Error, "Invalid input");
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
            return TaskType::StopApp;

        case 1:
            return TaskType::ListGames;

        case 2:
            return TaskType::ToggleGameClock;

        case 3:
            return TaskType::EditGamesMenu;

        case 4:
            return TaskType::AddNewGame;

        case 5:
            return TaskType::SettingsMenu;

        case 6:
            return TaskType::CheckForUpdates;

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}