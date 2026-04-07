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
#include <memory>

module Task_ValidateUserConfirmationUsedToResetGameClock;

import Task_ResetGameClock;
import Task_GetUserConfirmationUsedToResetGameClock;

using namespace gw::con::tasks;
using namespace gw::con::core;

ValidateUserConfirmationUsedToResetGameClock::ValidateUserConfirmationUsedToResetGameClock(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, console_{Task::ctx->console} {}

auto ValidateUserConfirmationUsedToResetGameClock::Run() noexcept -> std::unique_ptr<Task> {
    switch (console_.GetInputRequestStatus()) {
        case ConsoleComponents::InputRequestStatus::Success:
            return std::make_unique<ResetGameClock>(ctx);

        case ConsoleComponents::InputRequestStatus::Invalid:
            console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Invalid input");
            return std::make_unique<GetUserConfirmationUsedToResetGameClock>(ctx);

        default:
            assert(false && "Unhandled ConsoleComponents::InputRequestStatus in ValidateUserConfirmationUsedToResetGameClock::Run");
            std::terminate();
    }
}