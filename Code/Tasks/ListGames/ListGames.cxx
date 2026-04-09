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

module Tasks;
import :ListGames;

using namespace gw::con;

auto tasks::ListGames(core::Console& console, const core::GameLibraryReadAccess& game_library) noexcept -> core::TaskType {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(core::Console::MsgType::Error, "No entries found");
        return core::TaskType::MainMenu;
    }

    console.ClearScreen();
    game_library.ListGames();
    console.WriteLine(core::Console::MsgType::Tip, "Press any key to go back");
    core::Console::RequestKeyPress();
    return core::TaskType::MainMenu;
}