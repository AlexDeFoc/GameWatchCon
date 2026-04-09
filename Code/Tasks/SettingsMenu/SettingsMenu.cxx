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

module Tasks;
import :SettingsMenu;
import Utils;

using namespace gw::con;

auto tasks::SettingsMenu(core::Console& console, const core::AppConfig& app_config) noexcept -> core::TaskType {
    std::string autosave_status_text = "1. Toggle game clock autosave";
    if (app_config.GetAutoSaveStatus() == core::AppConfig::AutoSaveStatus::Enabled)
        // TODO: Optimize this stuff, its so insanely costly, computing each time, making allocations...
        autosave_status_text = std::format("{}: {}", autosave_status_text, core::utils::ColorText(console, core::utils::TextColor::Green, "enabled"));
    else
        autosave_status_text = std::format("{}: {}", autosave_status_text, core::utils::ColorText(console, core::utils::TextColor::Red, "disabled"));

    // TODO: Find a way to optimize this, there may not be a way though
    std::string autosave_interval_text = std::format("{} - {}", "2. Change game clock autosave interval", app_config.GetPrintableAutoSaveInterval());

    auto list_opts = [&] {
        std::println("{}", autosave_status_text);
        std::println("{}", autosave_interval_text);
        std::println("0. Go back");
    };

    [&] {
        while (true) {
            console.ClearScreen();
            console.WriteCachedMsgs();
            console.RequestMenuOptionID(list_opts, {0, 2}, core::Console::RequestIsCancellable::No);

            switch (console.GetInputRequestStatus()) {
                case core::Console::InputRequestStatus::Invalid:
                    console.WriteLineToCache(core::Console::MsgType::Error, "Invalid input");
                    break;

                case core::Console::InputRequestStatus::Success:
                    return;

                default:
                    assert(false && "Unhandled core::Console::InputRequestStatus");
                    std::terminate();
            }
        }
    }();

    switch (console.GetNumberInputResult()) {
        case 0:
            return core::TaskType::MainMenu;

        case 1:
            return core::TaskType::ToggleAutoSave;

        case 2:
            return core::TaskType::ChangeAutoSaveInterval;

        default:
            assert(false && "Unhandled option index");
            std::terminate();
    }
}