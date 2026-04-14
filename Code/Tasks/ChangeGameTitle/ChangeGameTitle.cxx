// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Tasks/ChangeGameTitle/ChangeGameTitle.hxx"

auto gw::tasks::ChangeGameTitle(Console& console, GameLibrary& game_library) noexcept -> TaskType {
    const auto next_task = [&] -> TaskType {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();

            const auto list_func = [&] { game_library.ListGames(console); };
            console.RequestGameID(list_func, {1, static_cast<int>(game_library.GamesCount())});

            switch (console.GetInputRequestStatus()) {
                case Console::InputRequestStatus::Success:
                    return TaskType::Default;

                case Console::InputRequestStatus::Cancelled:
                    console.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
                    return TaskType::EditGamesMenu;

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

    console.ClearScreen();
    console.WriteCachedMsgs();
    console.RequestGameTitle();
    switch (console.GetInputRequestStatus()) {
        case Console::InputRequestStatus::Success:
            break;

        case Console::InputRequestStatus::Cancelled:
            console.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
            return TaskType::EditGamesMenu;

        default:
            assert(false && "Unhandled Console::InputRequestStatus");
            std::terminate();
    }

    game_library.SetGameTitle(console.GetNumberInputResult() - 1, console.GetStringInputResult());
    console.WriteLineToCache(Console::MsgType::Info, "Changed game title");
    return TaskType::EditGamesMenu;
}