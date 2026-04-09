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
import :AddNewGame;

using namespace gw::con;

auto tasks::AddNewGame(core::Console& console, core::GameLibraryWriteAccess& game_library) noexcept -> core::TaskType {
    console.ClearScreen();
    console.RequestGameTitle();

    switch (console.GetInputRequestStatus()) {
        case core::Console::InputRequestStatus::Success:
            game_library.AddGame(console.GetStringInputResult());
            console.WriteLineToCache(core::Console::MsgType::Info, "Added game");
            return core::TaskType::MainMenu;

        case core::Console::InputRequestStatus::Cancelled:
            console.WriteLineToCache(core::Console::MsgType::Info, "Action cancelled");
            return core::TaskType::MainMenu;

        case core::Console::InputRequestStatus::Invalid:
            console.WriteLineToCache(core::Console::MsgType::Info, "An issue has occurred with adding the game");
            return core::TaskType::MainMenu;

        default:
            break;
    }

    assert(false && "Unhandled core::Console::InputRequestStatus in AddNewGame::Run");
    std::terminate();
}