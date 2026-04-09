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

export module App;

import AppConfig;
import Console;
import GameLibrary;
import TaskType;

export namespace gw::con::core {
class App {
public:
    App() noexcept;
    auto Run() noexcept -> void;

private:
    bool keep_app_running_;
    TaskType current_task_;
    AppConfig app_config_;
    Console console_;
    GameLibrary game_library_;
};
} // namespace gw::con::core