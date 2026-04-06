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
#include <memory>

export module Task;

import AppState;
import AppConfig;
import Console;

export namespace gw::con::core {
class Task {
public:
    struct Context {
        Context(AppState&, AppConfig&, Console&) noexcept;

        AppState& app_state;
        AppConfig& app_config;
        Console& console;
    };

    explicit Task(const std::shared_ptr<Context>&) noexcept;
    virtual ~Task() noexcept = default;
    [[nodiscard]] virtual auto Run() noexcept -> std::unique_ptr<Task> = 0;

protected:
    std::shared_ptr<Context> ctx;
};
} // namespace gw::con::core