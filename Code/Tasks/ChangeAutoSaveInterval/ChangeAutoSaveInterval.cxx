// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/ChangeAutoSaveInterval/ChangeAutoSaveInterval.hxx"

auto gw::tasks::ChangeAutoSaveInterval(Console& console, AppConfig& app_config) noexcept -> TaskType {
    // TODO: Display current interval
    console.ClearScreen();
    console.RequestAutoSaveInterval();
    app_config.ChangeAutoSaveInterval(std::chrono::seconds{console.GetNumberInputResult()});
    return TaskType::SettingsMenu;
}