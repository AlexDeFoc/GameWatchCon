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

import Task_GetEditGamesMenuOptionChoice;

using namespace gw::con::tasks;
using namespace gw::con::core;

ResetGameClock::ResetGameClock(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, game_library_{Task::ctx->game_library}, console_{Task::ctx->console} {}

auto ResetGameClock::Run() noexcept -> std::unique_ptr<Task> {
    console_.ClearScreen();
    const auto list_func = [&] { game_library_.ListGames(); };
    console_.RequestGameID(list_func, {1, game_library_.GamesCount()});

    switch (console_.GetInputRequestStatus()) {
        case ConsoleComponents::InputRequestStatus::Success:
            break;

        case ConsoleComponents::InputRequestStatus::Cancelled:
            console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Action cancelled");
            return std::make_unique<GetEditGamesMenuOptionChoice>(ctx);

        case ConsoleComponents::InputRequestStatus::Invalid:
            console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "An issue has occurred with resetting the game clock");
            return std::make_unique<GetEditGamesMenuOptionChoice>(ctx);

        default:
            assert(false && "Unhandled ConsoleComponents::InputRequestStatus in ResetGameClock::Run after requesting game id");
            std::terminate();
    }

    console_.ClearScreen();
    console_.RequestUserConfirmation();

    switch (console_.GetInputRequestStatus()) {
        case ConsoleComponents::InputRequestStatus::Success:
            break;

        case ConsoleComponents::InputRequestStatus::Cancelled:
            console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Action cancelled");
            return std::make_unique<GetEditGamesMenuOptionChoice>(ctx);

        case ConsoleComponents::InputRequestStatus::Invalid:
            console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "An issue has occurred with resetting the game clock");
            return std::make_unique<GetEditGamesMenuOptionChoice>(ctx);

        default:
            assert(false && "Unhandled ConsoleComponents::InputRequestStatus in Task_ChangeGameTitle::Run after requesting game title");
            std::terminate();
    }

    if (console_.GetUserConfirmationStatus() == true) {
        game_library_.ResetGameClock(console_.GetNumberInputResult() - 1);
        console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Game clock reset");
    } else {
        console_.WriteLineToCache(ConsoleComponents::MsgType::Info, "Action cancelled");
    }

    return std::make_unique<GetEditGamesMenuOptionChoice>(ctx);
}