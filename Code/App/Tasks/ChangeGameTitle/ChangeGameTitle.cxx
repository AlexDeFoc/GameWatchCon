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

module Task_ChangeGameTitle;

import Task_GetEditGamesMenuOptionChoice;

using namespace gw::con::tasks;
using namespace gw::con::core;

ChangeGameTitle::ChangeGameTitle(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, console_{Task::ctx->console}, game_library_{Task::ctx->game_library} {}

auto ChangeGameTitle::Run() noexcept -> std::unique_ptr<Task> {
    game_library_.SetGameTitle(console_.GetNumberInputResult() - 1, console_.GetStringInputResult());
    console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Changed game title");
    return std::make_unique<GetEditGamesMenuOptionChoice>(ctx);
}