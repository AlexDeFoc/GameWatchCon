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

import AppState;
import AppConfig;
import TaskQueue;
import Console;
import GameLibrary;

export namespace gw::con::core {
class App {
public:
    App() noexcept;
    auto Start() noexcept -> void;
    auto ProcessTask() noexcept -> void;

private:
    AppState app_state_;
    AppConfig app_config_;
    TaskQueue task_stack_;
    Console console_;
    GameLibrary game_library_;
};
} // namespace gw::con::core