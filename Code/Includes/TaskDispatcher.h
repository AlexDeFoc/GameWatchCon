// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "AppState.h"
#include "Console.h"
#include "DiskManager.h"
#include "AppSettings.h"
#include "GameLibrary.h"
#include "TaskType.h"

namespace gw {
class TaskDispatcher {
public:
    TaskDispatcher();
    auto Start() -> void;

private:
    TaskFunc next_task_;
    AppState app_state_;
    Console console_;
    DiskManager disk_manager_;
    AppSettings app_settings_;
    GameLibrary game_library_;
};
} // namespace gw