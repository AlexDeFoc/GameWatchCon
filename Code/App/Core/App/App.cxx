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

#include <memory>

module Core;
import :App;
import Tasks;

using namespace gw::con::core;
using namespace gw::con::tasks;

App::App() noexcept : app_state_{} {}

auto App::Start() noexcept -> void {
    task_stack_.Push(std::make_unique<StartApp>(std::make_shared<Task::Context>(app_state_, app_config_, console_)));

    do {
        ProcessTask();
    } while (app_state_.GetStatus() == AppStatusAccess::Status::Active);
}

auto App::ProcessTask() noexcept -> void {
    task_stack_.Push(task_stack_.Pop()->Run());
}