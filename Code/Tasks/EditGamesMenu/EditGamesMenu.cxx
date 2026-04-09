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

module Tasks;
import :EditGamesMenu;
import Console;

using namespace gw::con;

auto tasks::EditGamesMenu(core::Console& console, const core::GameLibraryReadAccess& game_library) noexcept -> core::TaskType {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(core::Console::MsgType::Error, "No entries found");
        return core::TaskType::MainMenu;
    }

    static auto list_opts = [] {
        std::println("1. Change game title");
        std::println("2. Reset game clock");
        std::println("3. Delete game");
        std::println("0. Go back");
    };

    [&] {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();
            console.RequestMenuOptionID(list_opts, {0, 3}, core::Console::RequestIsCancellable::No);

            switch (console.GetInputRequestStatus()) {
                case core::Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(core::Console::MsgType::Error, "Invalid input!");
                    break;

                case core::Console::InputRequestStatus::Success:
                    return;

                default:
                    assert(false && "Unhandled Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    switch (console.GetNumberInputResult()) {

        case 0:
            return core::TaskType::MainMenu;

        case 1:
            return core::TaskType::ChangeGameTitle;

        case 2:
            return core::TaskType::ResetGameClock;

        case 3:
            return core::TaskType::DeleteGame;

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}