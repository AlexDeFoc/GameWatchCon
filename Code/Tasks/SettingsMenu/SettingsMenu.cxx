// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/SettingsMenu/SettingsMenu.hxx"
#include "Core/Utils/Utils.hxx"

auto gw::tasks::SettingsMenu(Console& console, const AppConfig& app_config) noexcept -> TaskType {
    std::string autosave_status_text = "1. Toggle game clock autosave";
    if (app_config.GetAutoSaveStatus() == 1)
        // TODO: Optimize this stuff, its so insanely costly, computing each time, making allocations...
        autosave_status_text = std::format("{}: {}", autosave_status_text, utils::ColorText(console, utils::TextColor::Green, "enabled"));
    else
        autosave_status_text = std::format("{}: {}", autosave_status_text, utils::ColorText(console, utils::TextColor::Red, "disabled"));

    // TODO: Find a way to optimize this, there may not be a way though
    std::string autosave_interval_text = std::format("{} - {}", "2. Change game clock autosave interval", app_config.GetPrintableAutoSaveInterval());

    auto list_opts = [&] {
        std::println("{}", autosave_status_text);
        std::println("{}", autosave_interval_text);
        std::println("0. Go back");
    };

    [&] {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();
            console.RequestMenuOptionID(list_opts, {0, 2}, Console::RequestIsCancellable::No);

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
            return TaskType::MainMenu;

        case 1:
            return TaskType::ToggleAutoSave;

        case 2:
            return TaskType::ChangeAutoSaveInterval;

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}