// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Tasks/ChangeAutoSaveInterval/ChangeAutoSaveInterval.hxx"

auto gw::tasks::ChangeAutoSaveInterval(Console& console, AppConfig& app_config, const GameLibrary& game_library) noexcept -> TaskType {
    if (game_library.IsAnyGameActive()) {
        console.WriteLineToCache(Console::MsgType::Error, "Cannot change autosave interval while a game is active");
        return TaskType::SettingsMenu;
    }

    const auto next_task = [&] -> TaskType {
        while (true) {
            console.ClearScreen();
            console.WriteLine(Console::MsgType::Info, std::format("Current interval value: {}", app_config.GetPrintableAutoSaveInterval()));
            console.WriteCachedMsgs();
            console.RequestAutoSaveInterval();

            switch (console.GetInputRequestStatus()) {
                case Console::InputRequestStatus::Success:
                    return TaskType::Default;

                case Console::InputRequestStatus::Cancelled:
                    console.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
                    return TaskType::SettingsMenu;

                case Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(Console::MsgType::Error, "Invalid input");
                    break;

                default:
                    assert(false && "Unhandled Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    if (next_task != TaskType::Default)
        return next_task;

    app_config.ChangeAutoSaveInterval(std::chrono::seconds{console.GetNumberInputResult()});

    return TaskType::SettingsMenu;
}