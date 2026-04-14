// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/AppConfig/AppConfig.hxx"
#include "Core/AppState/AppState.hxx"
#include "Core/Console/Console.hxx"
#include "Core/GameLibrary/GameLibrary.hxx"
#include "Core/TaskType/TaskType.hxx"

namespace gw {
class App {
public:
    App() noexcept;
    auto Run() noexcept -> void;

private:
    AppState app_state_;
    TaskType current_task_;
    AppConfig app_config_;
    Console console_;
    GameLibrary game_library_;
};
} // namespace gw