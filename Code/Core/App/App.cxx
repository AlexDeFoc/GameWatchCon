// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/App/App.hxx"

#include "Tasks/Tasks.hxx"

gw::App::App() noexcept : keep_app_running_{false}, current_task_{TaskType::StartApp} {}

auto gw::App::Run() noexcept -> void {
    do {
        switch (current_task_) {
            case TaskType::AddNewGame:
                current_task_ = tasks::AddNewGame(console_, game_library_);
                break;

            case TaskType::ChangeAutoSaveInterval:
                current_task_ = tasks::ChangeAutoSaveInterval(console_, app_config_);
                break;

            case TaskType::ChangeGameTitle:
                current_task_ = tasks::ChangeGameTitle(console_, game_library_);
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
                current_task_ = tasks::MainMenu(console_);
                break;

            case TaskType::ResetGameClock:
                current_task_ = tasks::ResetGameClock(console_, game_library_);
                break;

            case TaskType::SettingsMenu:
                current_task_ = tasks::SettingsMenu(console_, app_config_);
                break;

            case TaskType::StartApp:
                current_task_ = tasks::StartApp(keep_app_running_);
                break;

            case TaskType::StopApp:
                current_task_ = tasks::StopApp(keep_app_running_);
                break;

            case TaskType::ToggleAutoSave:
                current_task_ = tasks::ToggleAutoSave(app_config_);
                break;

            case TaskType::Default:
                assert(false && "Invalid task handled");
                std::terminate();

            default:
                assert(false && "Unhandled task");
                std::terminate();
        }
    } while (keep_app_running_);
}