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
#include <format>

module Utils;

using namespace gw::con::core;

auto utils::ColorText(const Console& console, const TextColor color, std::string uncolored_text) noexcept -> std::string {
    if (console.IsCapableDisplayingColoredText() == false)
        return uncolored_text;

    switch (color) {
        case TextColor::DarkGray:
            return std::string{std::format("{}{}{}", "\x1b[90m", std::move(uncolored_text), "\x1b[0m")};
        case TextColor::Green:
            return std::string{std::format("{}{}{}", "\x1b[32m", std::move(uncolored_text), "\x1b[0m")};
        case TextColor::Magenta:
            return std::string{std::format("{}{}{}", "\x1b[35m", std::move(uncolored_text), "\x1b[0m")};
        case TextColor::Red:
            return std::string{std::format("{}{}{}", "\x1b[31m", std::move(uncolored_text), "\x1b[0m")};

        default:
            assert(false && "Unhandled text color");
            std::terminate();
    }
}