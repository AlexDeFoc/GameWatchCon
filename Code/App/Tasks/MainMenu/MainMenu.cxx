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

module Task_MainMenu;

import Task_StopApp;
import Task_ListGames;
import Task_AddNewGame;
import Task_GetEditGamesMenuOptionChoice;

using namespace gw::con::tasks;
using namespace gw::con::core;

MainMenu::MainMenu(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, console_{Task::ctx->console} {}

auto MainMenu::Run() noexcept -> std::unique_ptr<Task> {
    static auto list_opts = [] {
        std::println("1. List games");
        std::println("2. Start game");
        std::println("3. Edit games");
        std::println("4. Add new game");
        std::println("5. Settings");
        std::println("6. Check for updates");
        std::println("0. Exit app");
    };

    [&] {
        while (true) {
            console_.ClearScreen();
            console_.WriteCachedMsgs();
            console_.RequestMenuOptionID(list_opts, {0, 6}, ConsoleComponents::RequestIsCancellable::No);

            switch (console_.GetInputRequestStatus()) {
                case ConsoleComponents::InputRequestStatus::Invalid:
                    console_.WriteLineToCache(ConsoleComponents::MsgType::Error, "Invalid input");
                    break;

                case ConsoleComponents::InputRequestStatus::Success:
                    return;

                default:
                    assert(false && "Unhandled ConsoleComponents::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    switch (console_.GetNumberInputResult()) {
        case 0:
            return std::make_unique<StopApp>(ctx);

        case 1:
            return std::make_unique<ListGames>(ctx);

        case 3:
            return std::make_unique<GetEditGamesMenuOptionChoice>(ctx);

        case 4:
            return std::make_unique<AddNewGame>(ctx);

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}