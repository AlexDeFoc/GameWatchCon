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
#include <vector>

module Core;
import :TaskQueue;

using namespace gw::con::core;

auto TaskQueue::Push(std::unique_ptr<Task> new_task) noexcept -> void { tasks_.push_back(std::move(new_task)); };

auto TaskQueue::PushReversedList(std::vector<std::unique_ptr<Task>> new_tasks) noexcept -> void {
    for (auto it = new_tasks.rbegin(); it != new_tasks.rend(); ++it)
        tasks_.emplace_back(std::move(*it));
}

auto TaskQueue::Pop() noexcept -> std::unique_ptr<Task> {
    if (tasks_.empty())
        return nullptr;

    auto top = std::move(tasks_.back());
    tasks_.pop_back();
    return top;
}

auto TaskQueue::InspectTop() const noexcept -> const Task* {
    if (tasks_.empty())
        return nullptr;

    return tasks_.back().get();
}