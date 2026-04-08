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

module Task_SettingsMenu;

import Task_MainMenu;
import Task_ToggleAutoSave;

using namespace gw::con::tasks;
using namespace gw::con::core;

SettingsMenu::SettingsMenu(const std::shared_ptr<Context>& ctx) noexcept : Task{ctx}, console_{Task::ctx->console}, app_config_{Task::ctx->app_config} {}

auto SettingsMenu::Run() noexcept -> std::unique_ptr<Task> {
    // TODO: Change from true to 'Enabled' enum typed value
    // TODO 2: Make it colored the 'enabled' text value and diff color from disabled (need to impl console color text method)
    std::string autosave_status_text = "1. Toggle game clock autosave";
    if (app_config_.GetAutoSaveStatus() == true)
        autosave_status_text = std::format("{}: enabled", autosave_status_text);
    else
        autosave_status_text = std::format("{}: disabled", autosave_status_text);

    auto list_opts = [&] {
        std::println("{}", autosave_status_text);
        std::println("2. Change game clock autosave interval");
        std::println("0. Go back");
    };

    [&] {
        while (true) {
            console_.ClearScreen();
            console_.WriteCachedMsgs();
            console_.RequestMenuOptionID(list_opts, {0, 2}, Console::RequestIsCancellable::No);

            switch (console_.GetInputRequestStatus()) {
                case Console::InputRequestStatus::Invalid:
                    console_.WriteLineToCache(Console::MsgType::Error, "Invalid input");
                    break;

                case Console::InputRequestStatus::Success:
                    return;

                default:
                    assert(false && "Unhandled Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    switch (console_.GetNumberInputResult()) {
        case 0:
            return std::make_unique<MainMenu>(ctx);

        case 1:
            return std::make_unique<ToggleAutoSave>(ctx);

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}