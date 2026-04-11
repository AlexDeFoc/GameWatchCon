// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Tasks/ToggleAutoSave/ToggleAutoSave.hxx"

auto gw::tasks::ToggleAutoSave(AppConfig& app_config) noexcept -> TaskType {
    app_config.ToggleAutoSaveStatus();

    return TaskType::SettingsMenu;
}