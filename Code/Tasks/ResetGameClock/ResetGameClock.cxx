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
#include <exception>

module Tasks;
import :ResetGameClock;

using namespace gw::con;

auto tasks::ResetGameClock(core::Console& console, core::GameLibrary& game_library) noexcept -> core::TaskType {
    const auto next_task = [&] -> core::TaskType {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();

            const auto list_func = [&] { game_library.ListGames(); };
            console.RequestGameID(list_func, {1, game_library.GamesCount()});

            switch (console.GetInputRequestStatus()) {
                case core::Console::InputRequestStatus::Success:
                    return core::TaskType::Default;

                case core::Console::InputRequestStatus::Cancelled:
                    console.WriteLineToCache(core::Console::MsgType::Info, "Action cancelled");
                    return core::TaskType::EditGamesMenu;

                case core::Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(core::Console::MsgType::Info, "Invalid input");
                    break;

                default:
                    assert(false && "Unhandled core::Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    if (next_task != core::TaskType::Default)
        return next_task;

    [&] {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();
            console.RequestUserConfirmation();

            switch (console.GetInputRequestStatus()) {
                case core::Console::InputRequestStatus::Success:
                    return;

                case core::Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(core::Console::MsgType::Info, "Invalid input");
                    break;

                default:
                    assert(false && "Unhandled core::Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    if (console.GetUserConfirmationStatus() == true) {
        game_library.ResetGameClock(console.GetNumberInputResult() - 1);
        console.WriteLineToCache(core::Console::MsgType::Info, "Reset game clock");
    } else {
        console.WriteLineToCache(core::Console::MsgType::Info, "Action cancelled");
    }

    return core::TaskType::EditGamesMenu;
}