// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/ToggleGameClock/ToggleGameClock.hxx"

auto gw::tasks::ToggleGameClock(GameLibrary& game_library) noexcept -> TaskType {
    game_library.ToggleGameClock();

    return TaskType::MainMenu;
}