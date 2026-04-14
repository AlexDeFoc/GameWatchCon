// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Tasks/ToggleAutoSave/ToggleAutoSave.hxx"

auto gw::tasks::ToggleAutoSave(Console& console, AppConfig& app_config, const GameLibrary& game_library) noexcept -> TaskType {
    if (game_library.IsAnyGameActive())
        console.WriteLineToCache(Console::MsgType::Error, "Cannot toggle autosave status while a game is active");
    else
        app_config.ToggleAutoSaveStatus();

    return TaskType::SettingsMenu;
}