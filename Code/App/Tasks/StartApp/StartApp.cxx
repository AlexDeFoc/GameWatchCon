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
#include <optional>
#include <vector>

module Tasks;
import :StartApp;
import :StopApp;

using namespace gw::con::tasks;
using namespace gw::con::core;

StartApp::StartApp(Context ctx) noexcept : Task{Kind::Bundle, std::move(ctx)}, app_status_{Task::ctx.app_state} {}

auto StartApp::Run() noexcept -> void { app_status_.SetStatus(AppStatusAccess::Status::Active); }

auto StartApp::ExpandSelf() noexcept -> std::optional<std::vector<std::unique_ptr<Task>>> {
    std::vector<std::unique_ptr<Task>> bundle{};
    bundle.emplace_back(std::make_unique<StopApp>(std::move(ctx)));
    return bundle;
}