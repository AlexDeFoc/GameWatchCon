/*
    GameWatchCon - Keep track of your in-game time
    Copyright (C) 2026  Sava Alexandru-Andrei

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

module;

#include <cassert>
#include <exception>

module App;

import TaskType;
import Tasks;

using namespace gw::con::core;
using namespace gw::con;

App::App() noexcept : keep_app_running_{false}, current_task_{TaskType::StartApp} {}

auto App::Run() noexcept -> void {
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