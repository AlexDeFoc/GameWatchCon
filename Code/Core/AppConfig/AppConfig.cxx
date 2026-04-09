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

module AppConfig;

using namespace gw::con::core;
using namespace std::chrono_literals;
using namespace std::chrono;

// TODO: change autosave to being true by default
AppConfig::AppConfig() noexcept : autosave_enabled_status_{false}, autosave_interval_{5min} {}

auto AppConfig::ToggleAutoSaveStatus() noexcept -> void { autosave_enabled_status_ = !autosave_enabled_status_; }

auto AppConfig::ChangeAutoSaveInterval(const steady_clock::duration new_interval) noexcept -> void { autosave_interval_ = new_interval; }

auto AppConfig::GetAutoSaveStatus() const noexcept -> bool { return autosave_enabled_status_; }

// TODO: Make this more/stronger type/d safe
auto AppConfig::GetPrintableAutoSaveInterval() const noexcept -> std::string {
    auto ms = duration_cast<milliseconds>(autosave_interval_);
    const auto d = duration_cast<days>(ms);
    ms -= d;
    const auto h = duration_cast<hours>(ms);
    ms -= h;
    const auto m = duration_cast<minutes>(ms);
    ms -= m;
    const auto s = duration_cast<seconds>(ms);
    ms -= s;

    std::vector<std::string> bits{};
    if (d.count() > 0)
        bits.emplace_back(std::format("{} days", d.count()));
    if (h.count() > 0)
        bits.emplace_back(std::format("{} h", h.count()));
    if (m.count() > 0)
        bits.emplace_back(std::format("{} min", m.count()));
    if (s.count() > 0)
        bits.emplace_back(std::format("{} s", s.count()));
    if (ms.count() > 0)
        bits.emplace_back(std::format("{} ms", ms.count()));

    if (bits.empty())
        return "0 s";

    std::string printable_duration{};
    for (std::size_t i{0}; i < bits.size(); ++i) {
        printable_duration += bits[i];

        if (i < bits.size() - 1)
            printable_duration += " : ";
    }

    return printable_duration;
}