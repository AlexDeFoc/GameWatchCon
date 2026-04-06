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

#include <string>
#include <vector>

export module Core:ConsoleWriteAccess;
import :ConsoleComponents;

export namespace gw::con::core {
class ConsoleWriteAccess {
public:
    ConsoleWriteAccess() noexcept = default;
    auto WriteLineToCache(ConsoleComponents::MsgType, std::string_view) noexcept -> void; // TODO: Check usage (may need to remove)
    auto WriteToCache(ConsoleComponents::MsgType, std::string_view) noexcept -> void; // TODO: Check usage (may need to remove)
    auto WriteCachedMsgs() noexcept -> void;

    static auto WriteLine(ConsoleComponents::MsgType, std::string_view) noexcept -> void;
    static auto Write(ConsoleComponents::MsgType, std::string_view) noexcept -> void;
    static auto ClearScreen() noexcept -> void;

private:
    [[nodiscard]] static auto GetDecoratedMsgTag(ConsoleComponents::MsgType) noexcept -> std::string;
    [[nodiscard]] static auto GetMsgTag(ConsoleComponents::MsgType) noexcept -> std::string_view;
    [[nodiscard]] static auto GetMsgTagColor(ConsoleComponents::MsgType) noexcept -> std::string_view;

    std::vector<std::string> cached_msgs_;
};
} // namespace gw::con::core