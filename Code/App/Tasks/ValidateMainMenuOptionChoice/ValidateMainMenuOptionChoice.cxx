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
#include <print>

module Task_ValidateMainMenuOptionChoice;

import Task_GetMainMenuOptionChoice;
import Task_ForwardUserFromMainMenu;

using namespace gw::con::tasks;
using namespace gw::con::core;

ValidateMainMenuOptionChoice::ValidateMainMenuOptionChoice(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, console{Task::ctx->console} {}

auto ValidateMainMenuOptionChoice::Run() noexcept -> std::unique_ptr<Task> {
    switch (console.GetInputRequestStatus()) {
        case ConsoleComponents::InputRequestStatus::Invalid:
            console.WriteLineToCache(ConsoleComponents::MsgType::Error, "Invalid input!");
            return std::make_unique<GetMainMenuOptionChoice>(ctx);

        case ConsoleComponents::InputRequestStatus::Success:
            return std::make_unique<ForwardUserFromMainMenu>(ctx);

        default:
            break;
    }

    assert(false && "Unhandled ConsoleComponents::InputRequestStatus in ValidateMainMenuOptionChoice::Run");
    std::terminate();
}