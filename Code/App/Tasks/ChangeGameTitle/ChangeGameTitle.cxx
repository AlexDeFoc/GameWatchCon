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

module Task_ChangeGameTitle;

import Task_EditGamesMenu;

using namespace gw::con::tasks;
using namespace gw::con::core;

ChangeGameTitle::ChangeGameTitle(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, console_{Task::ctx->console}, game_library_{Task::ctx->game_library} {}

auto ChangeGameTitle::Run() noexcept -> std::unique_ptr<Task> {
    auto next_task = [&] -> std::unique_ptr<Task> {
        while (true) {
            console_.ClearScreen();
            console_.WriteCachedMsgs();

            const auto list_func = [&] { game_library_.ListGames(); };
            console_.RequestGameID(list_func, {1, game_library_.GamesCount()});

            switch (console_.GetInputRequestStatus()) {
                case Console::InputRequestStatus::Success:
                    return nullptr;

                case Console::InputRequestStatus::Cancelled:
                    console_.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
                    return std::make_unique<EditGamesMenu>(ctx);

                case Console::InputRequestStatus::Invalid:
                    console_.WriteLineToCache(Console::MsgType::Info, "Invalid input");
                    break;

                default:
                    assert(false && "Unhandled Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    if (next_task != nullptr)
        return next_task;

    console_.ClearScreen();
    console_.WriteCachedMsgs();
    console_.RequestGameTitle();
    switch (console_.GetInputRequestStatus()) {
        case Console::InputRequestStatus::Success:
            break;

        case Console::InputRequestStatus::Cancelled:
            console_.WriteLineToCache(Console::MsgType::Info, "Action cancelled");
            return std::make_unique<EditGamesMenu>(ctx);

        default:
            assert(false && "Unhandled Console::InputRequestStatus");
            std::terminate();
    }

    game_library_.SetGameTitle(console_.GetNumberInputResult() - 1, console_.GetStringInputResult());
    console_.WriteLineToCache(Console::MsgType::Info, "Changed game title");
    return std::make_unique<EditGamesMenu>(ctx);
}