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

App::App() noexcept : app_state_{}, app_config_{}, task_stack_{}, console_{} {}

auto App::Start() noexcept -> void {
    app_state_.SetStatus(AppStatusAccess::Status::Active);
    task_stack_.Push(std::make_unique<StartApp>(Task::Context{app_state_, app_config_, console_}));

    while (app_state_.GetStatus() == AppStatusAccess::Status::Active) {
        ExpandTask();
        ProcessTask();
    }
}

auto App::ExpandTask() noexcept -> void {
    if (task_stack_.InspectTop() == nullptr) {
        task_stack_.Push(std::make_unique<StopApp>(Task::Context{app_state_, app_config_, console_}));
        return;
    }

    if (task_stack_.InspectTop()->GetKind() == Task::Kind::Bundle) {
        task_stack_.PushReversedList(*((task_stack_.Pop())->ExpandSelf()));
    }
}

auto App::ProcessTask() noexcept -> void {
    if (const auto task = task_stack_.Pop())
        task->Run();
}