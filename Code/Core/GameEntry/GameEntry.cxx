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
#include <string>

module GameEntry;

using namespace gw::con::core;

GameEntry::GameEntry() noexcept = default;

GameEntry::GameEntry(std::string title) noexcept : title_{std::move(title)} {}

auto GameEntry::SetTitle(std::string new_title) noexcept -> void { title_ = std::move(new_title); }

auto GameEntry::GetTitle() const noexcept -> std::string_view { return title_; }

auto GameEntry::AddTime(std::chrono::steady_clock::duration additional_time) noexcept -> void { clock_.AddTime(std::move(additional_time)); }

auto GameEntry::ResetClock() noexcept -> void { clock_.Reset(); }

auto GameEntry::GetPrintableClock() const noexcept -> std::string {
    std::vector<std::string> clock_bits{};

    if (clock_.GetDays() > std::chrono::days::zero())
        clock_bits.emplace_back(std::format("{} days", clock_.GetDays()));
    if (clock_.GetHours() > std::chrono::hours::zero())
        clock_bits.emplace_back(std::format("{} h", clock_.GetHours()));
    if (clock_.GetMinutes() > std::chrono::minutes::zero())
        clock_bits.emplace_back(std::format("{} min", clock_.GetMinutes()));
    if (clock_.GetSeconds() > std::chrono::seconds::zero())
        clock_bits.emplace_back(std::format("{} s", clock_.GetSeconds()));
    if (clock_.GetMilliseconds() > std::chrono::milliseconds::zero())
        clock_bits.emplace_back(std::format("{} ms", clock_.GetMilliseconds()));

    if (clock_bits.empty())
        return "0 s";

    std::string printable_clock{};
    for (std::size_t i{0}; i < clock_bits.size(); ++i) {
        printable_clock += clock_bits[i];

        if (i < clock_bits.size() - 1)
            printable_clock += " : ";
    }

    return printable_clock;
}