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

#include <cstdint>

export module GameClock;

export namespace gw::con::core {
class GameClock {
public:
    GameClock() noexcept;

private:
    std::uint16_t days_count_;
    std::uint8_t hours_count_;
    std::uint8_t minutes_count_;
    std::uint8_t seconds_count_;
    std::uint16_t milliseconds_count_;
};
} // namespace gw::con::core