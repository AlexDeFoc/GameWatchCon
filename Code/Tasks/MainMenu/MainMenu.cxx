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
import :MainMenu;
import Console;

using namespace gw::con;

auto tasks::MainMenu(core::Console& console) noexcept -> core::TaskType {
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
            console.ClearScreen();
            console.WriteCachedMsgs();
            console.RequestMenuOptionID(list_opts, {0, 6}, core::Console::RequestIsCancellable::No);

            switch (console.GetInputRequestStatus()) {
                case core::Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(core::Console::MsgType::Error, "Invalid input");
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
            return core::TaskType::StopApp;

        case 1:
            return core::TaskType::ListGames;

        case 3:
            return core::TaskType::EditGamesMenu;

        case 4:
            return core::TaskType::AddNewGame;

        case 5:
            return core::TaskType::SettingsMenu;

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}