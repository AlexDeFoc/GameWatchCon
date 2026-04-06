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

module TaskQueue;

using namespace gw::con::core;

auto TaskQueue::Push(std::unique_ptr<Task> new_task) noexcept -> void { tasks_.push(std::move(new_task)); };

auto TaskQueue::Pop() noexcept -> std::unique_ptr<Task> {
    if (tasks_.empty())
        return nullptr;

    auto top = std::move(tasks_.front());
    tasks_.pop();
    return top;
}