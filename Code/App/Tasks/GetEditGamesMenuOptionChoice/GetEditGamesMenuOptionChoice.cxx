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

#include <print>

module Task_GetEditGamesMenuOptionChoice;

import Task_GetMainMenuOptionChoice;
import Task_ValidateEditGamesMenuOptionChoice;

using namespace gw::con::tasks;
using namespace gw::con::core;

GetEditGamesMenuOptionChoice::GetEditGamesMenuOptionChoice(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, game_library_{Task::ctx->game_library}, console_{Task::ctx->console} {}

auto GetEditGamesMenuOptionChoice::Run() noexcept -> std::unique_ptr<Task> {
    if (game_library_.IsEmpty()) {
        console_.WriteLineToCache(ConsoleComponents::MsgType::Error, "No entries found");
        return std::make_unique<GetMainMenuOptionChoice>(ctx);
    }

    static auto list_opts = [] {
        std::println("1. Change game title");
        std::println("2. Reset game clock");
        std::println("3. Delete game");
        std::println("0. Go back");
    };

    console_.ClearScreen();
    console_.WriteCachedMsgs();
    console_.RequestMenuOptionID(list_opts, {0, 3}, ConsoleComponents::RequestIsCancellable::No);

    return std::make_unique<ValidateEditGamesMenuOptionChoice>(ctx);
}