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
#include <string>

module Task_AddNewGame;

import Task_MainMenu;

using namespace gw::con::tasks;
using namespace gw::con::core;

AddNewGame::AddNewGame(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, console_{Task::ctx->console}, game_library_{Task::ctx->game_library} {}

auto AddNewGame::Run() noexcept -> std::unique_ptr<Task> {
    console_.ClearScreen();
    console_.RequestGameTitle();

    switch (console_.GetInputRequestStatus()) {
        case Console::InputRequestStatus::Success:
            game_library_.AddGame(console_.GetStringInputResult());
            console_.WriteLineToCache(Console::MsgType::Info, "Added game");
            return std::make_unique<MainMenu>(ctx);

        case Console::InputRequestStatus::Cancelled:
            console_.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
            return std::make_unique<MainMenu>(ctx);

        case Console::InputRequestStatus::Invalid:
            console_.WriteLineToCache(Console::MsgType::Info, "An issue has occurred with adding the game");
            return std::make_unique<MainMenu>(ctx);

        default:
            break;
    }

    assert(false && "Unhandled Console::InputRequestStatus in AddNewGame::Run");
    std::terminate();
}