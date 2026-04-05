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

#include <utility>

module Core;
import :Task;

using namespace gw::con::core;

Task::Context::Context(AppState& app_state, AppConfig& app_config) noexcept : app_state{app_state}, app_config{app_config} {}

Task::Task(const Kind kind, Context ctx) noexcept : ctx{std::move(ctx)}, kind_{kind} {}

auto Task::GetKind() const noexcept -> Kind { return kind_; }