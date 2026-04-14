// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "pch.hxx"
#include "Core/App/App.hxx"
#include "Tasks/Tasks.hxx"

gw::App::App() noexcept : current_task_{TaskType::StartApp}, app_config_{}, game_library_{app_state_, app_config_} {}

auto gw::App::Run() noexcept -> void {
    do {
        switch (current_task_) {
            case TaskType::AddNewGame:
                current_task_ = tasks::AddNewGame(console_, game_library_);
                break;

            case TaskType::ChangeAutoSaveInterval:
                current_task_ = tasks::ChangeAutoSaveInterval(console_, app_config_, game_library_);
                break;

            case TaskType::ChangeGameTitle:
                current_task_ = tasks::ChangeGameTitle(console_, game_library_);
                break;

            case TaskType::CheckForUpdates:
                current_task_ = tasks::CheckForUpdates(console_);
                break;

            case TaskType::DeleteGame:
                current_task_ = tasks::DeleteGame(console_, game_library_);
                break;

            case TaskType::EditGamesMenu:
                current_task_ = tasks::EditGamesMenu(console_, game_library_);
                break;

            case TaskType::ListGames:
                current_task_ = tasks::ListGames(console_, game_library_);
                break;

            case TaskType::MainMenu:
                current_task_ = tasks::MainMenu(console_, game_library_);
                break;

            case TaskType::ResetGameClock:
                current_task_ = tasks::ResetGameClock(console_, game_library_);
                break;

            case TaskType::SettingsMenu:
                current_task_ = tasks::SettingsMenu(console_, app_config_);
                break;

            case TaskType::StartApp:
                current_task_ = tasks::StartApp();
                break;

            case TaskType::StopApp:
                current_task_ = tasks::StopApp(app_state_);
                break;

            case TaskType::ToggleAutoSave:
                current_task_ = tasks::ToggleAutoSave(console_, app_config_, game_library_);
                break;

            case TaskType::ToggleGameClock:
                current_task_ = tasks::ToggleGameClock(console_, game_library_);
                break;

            case TaskType::Default:
                assert(false && "Invalid task handled");
                std::terminate();

            default:
                assert(false && "Unhandled task");
                std::terminate();
        }
    } while (app_state_.IsAppRunning());
}