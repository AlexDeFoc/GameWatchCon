// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

namespace gw {
enum class TaskType {
    Default, // TODO: Remove
    AddNewGame,
    ChangeAutoSaveInterval,
    ChangeGameTitle,
    CheckForUpdates,
    DeleteGame,
    EditGamesMenu,
    ListGames,
    MainMenu,
    ResetGameClock,
    SettingsMenu,
    StartApp,
    StopApp,
    ToggleAutoSave,
    ToggleGameClock
};
}