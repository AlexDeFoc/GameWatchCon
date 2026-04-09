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

#include <chrono>

export module GameClock;

export namespace gw::con::core {
class GameClock {
public:
    GameClock() noexcept;

    auto AddTime(std::chrono::steady_clock::duration) noexcept -> void;
    auto Reset() noexcept -> void;
    [[nodiscard]] auto GetDays() const noexcept -> std::chrono::days;
    [[nodiscard]] auto GetHours() const noexcept -> std::chrono::hours;
    [[nodiscard]] auto GetMinutes() const noexcept -> std::chrono::minutes;
    [[nodiscard]] auto GetSeconds() const noexcept -> std::chrono::seconds;
    [[nodiscard]] auto GetMilliseconds() const noexcept -> std::chrono::milliseconds;

private:
    std::chrono::steady_clock::duration duration_;
};
} // namespace gw::con::core