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

#include <optional>
#include <print>

module Tasks;
import :ListMainMenuOptions;

using namespace gw::con::tasks;
using namespace gw::con::core;

ListMainMenuOptions::ListMainMenuOptions(Context ctx) noexcept : Task{Kind::Unit, std::move(ctx)}, console_write_access_{Task::ctx.console} {}

auto ListMainMenuOptions::Run() noexcept -> void {
    console_write_access_.ClearScreen();

    std::println("1. List games");
    std::println("2. Edit games");
    std::println("3. Add new game");
    std::println("4. Settings");
    std::println("5. Check for updates");
    std::println("0. Exit app");
}

auto ListMainMenuOptions::ExpandSelf() noexcept -> std::optional<std::vector<std::unique_ptr<Task>>> { return {}; }