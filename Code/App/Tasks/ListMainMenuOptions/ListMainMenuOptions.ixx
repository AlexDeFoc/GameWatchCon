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

export module Tasks:ListMainMenuOptions;

import Core;

export namespace gw::con::tasks {
class ListMainMenuOptions : public core::Task {
public:
    explicit ListMainMenuOptions(Context) noexcept;
    auto Run() noexcept -> void override;
    [[nodiscard]] auto ExpandSelf() noexcept -> std::optional<std::vector<std::unique_ptr<Task>>> override;

private:
    core::ConsoleWriteAccess& console_write_access_;
};
} // namespace gw::con::tasks