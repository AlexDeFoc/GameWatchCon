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

export module Core:TaskQueue;

import :Task;

export namespace gw::con::core {
class TaskQueue {
public:
    TaskQueue() noexcept = default;
    auto Push(std::unique_ptr<Task>) noexcept -> void;
    auto PushReversedList(std::vector<std::unique_ptr<Task>>) noexcept -> void;
    [[nodiscard]] auto Pop() noexcept -> std::unique_ptr<Task>;
    [[nodiscard]] auto InspectTop() const noexcept -> const Task*;

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};
} // namespace gw::con::core