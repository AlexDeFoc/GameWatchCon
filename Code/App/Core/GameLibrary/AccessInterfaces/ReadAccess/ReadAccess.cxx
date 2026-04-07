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

module GameLibrary;
import :ReadAccess;

using namespace gw::con::core;

auto GameLibraryReadAccess::ListGames() const noexcept -> void {
    for (std::size_t game_index{1}; const auto& game : games_) {
        std::println("{}. {} - {}", game_index, game.GetTitle(), game.GetPrintableClock());
        game_index++;
    }
}

auto GameLibraryReadAccess::IsEmpty() const noexcept -> bool { return games_.empty(); }

auto GameLibraryReadAccess::GetGameTitle(const std::size_t index) const noexcept -> std::string_view {
    assert(index < games_.size() && "Provided out of range index to GameLibraryReadAccess::GetGameTitle");
    return games_[index].GetTitle();
}