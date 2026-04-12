// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/ToggleGameClock/ToggleGameClock.hxx"

auto gw::tasks::ToggleGameClock(Console& console, GameLibrary& game_library) noexcept -> TaskType {
    if (!game_library.IsAnyGameActive()) {
        if (game_library.IsEmpty()) {
            console.WriteLineToCache(Console::MsgType::Error, "No entries found");
            return TaskType::MainMenu;
        }

        const auto next_task = [&] -> TaskType {
            while (true) {
                console.ClearScreen();
                console.WriteCachedMsgs();

                const auto list_func = [&] { game_library.ListGames(); };
                console.RequestGameID(list_func, {1, static_cast<int>(game_library.GamesCount())});

                switch (console.GetInputRequestStatus()) {
                    case Console::InputRequestStatus::Success:
                        return TaskType::Default;

                    case Console::InputRequestStatus::Cancelled:
                        console.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
                        return TaskType::MainMenu;

                    case Console::InputRequestStatus::Invalid:
                        console.WriteLineToCache(Console::MsgType::Info, "Invalid input");
                        break;

                    default:
                        assert(false && "Unhandled Console::InputRequestStatus");
                        std::terminate();
                }
            }
        }();

        if (next_task != TaskType::Default)
            return next_task;

        game_library.ToggleGameClock(console.GetNumberInputResult() - 1);
    } else {
        game_library.ToggleGameClock(std::nullopt);
    }

    return TaskType::MainMenu;
}