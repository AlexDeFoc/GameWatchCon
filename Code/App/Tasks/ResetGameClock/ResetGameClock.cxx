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

module Task_ResetGameClock;

import Task_EditGamesMenu;

using namespace gw::con::tasks;
using namespace gw::con::core;

ResetGameClock::ResetGameClock(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, game_library_{Task::ctx->game_library}, console_{Task::ctx->console} {}

auto ResetGameClock::Run() noexcept -> std::unique_ptr<Task> {
    if (console_.GetUserConfirmationStatus() == true) {
        game_library_.ResetGameClock(console_.GetNumberInputResult() - 1);
        console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Reset game clock");
    } else {
        console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Action cancelled");
    }

    return std::make_unique<EditGamesMenu>(ctx);
}