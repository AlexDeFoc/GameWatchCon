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

#include <chrono>

export module AppConfig;

export namespace gw::con::core {
class AppConfig {
public:
    AppConfig() noexcept;

    auto ToggleAutoSaveStatus() noexcept -> void;
    auto ChangeAutoSaveInterval(std::chrono::steady_clock::duration) noexcept -> void;
    [[nodiscard]] auto GetAutoSaveStatus() const noexcept -> bool;
    [[nodiscard]] auto GetPrintableAutoSaveInterval() const noexcept -> std::string;

private:
    bool autosave_enabled_status_;
    std::chrono::steady_clock::duration autosave_interval_;
};
} // namespace gw::con::core