// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Console.hpp"
#include "GameLibrary.hpp"
#include "AppSettings.hpp"
#include "Tasks/RestoreSettingsDefaults.hpp"
#include "Tasks/SettingsMenu.hpp"

auto gw::tasks::RestoreSettingsDefaults(gw::Console& console, gw::AppState&, gw::AppSettings& app_settings, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsAnyGameActive()) {
        console.WriteLineToCache(Console::Tag::Error, "Cannot restore settings to defaults while a game is active. Stop it first!");
        return Task{gw::tasks::SettingsMenu};
    }

    const auto [user_is_sure, input_status_for_user_confirmation] = console.RequestUserConfirmation();

    switch (input_status_for_user_confirmation) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    if (user_is_sure) {
        app_settings.RestoreSettingsDefaults();
        console.WriteLineToCache(Console::Tag::Success, "Restored all settings defaults");
    } else
        console.WriteLineToCache(Console::Tag::Info, "Action cancelled");

    return Task{gw::tasks::SettingsMenu};
}

