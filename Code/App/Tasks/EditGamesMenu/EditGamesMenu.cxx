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

module Task_EditGamesMenu;

import Task_MainMenu;
import Task_ChangeGameTitle;
import Task_ResetGameClock;
import Task_DeleteGame;

using namespace gw::con::tasks;
using namespace gw::con::core;

EditGamesMenu::EditGamesMenu(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, game_library_{Task::ctx->game_library}, console_{Task::ctx->console} {}

auto EditGamesMenu::Run() noexcept -> std::unique_ptr<Task> {
    if (game_library_.IsEmpty()) {
        console_.WriteLineToCache(Console::MsgType::Error, "No entries found");
        return std::make_unique<MainMenu>(ctx);
    }

    static auto list_opts = [] {
        std::println("1. Change game title");
        std::println("2. Reset game clock");
        std::println("3. Delete game");
        std::println("0. Go back");
    };

    [&] {
        while (true) {
            console_.ClearScreen();
            console_.WriteCachedMsgs();
            console_.RequestMenuOptionID(list_opts, {0, 3}, Console::RequestIsCancellable::No);

            switch (console_.GetInputRequestStatus()) {
                case Console::InputRequestStatus::Invalid:
                    console_.WriteLineToCache(Console::MsgType::Error, "Invalid input!");
                    break;

                case Console::InputRequestStatus::Success:
                    return;

                default:
                    assert(false && "Unhandled Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    switch (console_.GetNumberInputResult()) {
        case 0:
            return std::make_unique<MainMenu>(ctx);

        case 1:
            return std::make_unique<ChangeGameTitle>(ctx);

        case 2:
            return std::make_unique<ResetGameClock>(ctx);

        case 3:
            return std::make_unique<DeleteGame>(ctx);

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}