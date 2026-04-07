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

module Console;
import :ConsoleWriteAccess;

using namespace gw::con::core;

auto ConsoleWriteAccess::WriteLineToCache(const ConsoleComponents::MsgType msg_type, std::string_view msg) noexcept -> void { cached_msgs_.push_back(std::format("{}: {}\n", GetDecoratedMsgTag(msg_type), msg)); }

auto ConsoleWriteAccess::WriteToCache(const ConsoleComponents::MsgType msg_type, std::string_view msg) noexcept -> void { cached_msgs_.push_back(std::format("{}: {}", GetDecoratedMsgTag(msg_type), msg)); }

auto ConsoleWriteAccess::WriteCachedMsgs() noexcept -> void {
    for (const auto& msg : cached_msgs_)
        std::print("{}", msg);

    cached_msgs_.clear();
}

// Static Methods
auto ConsoleWriteAccess::ClearScreen() noexcept -> void {
    std::print("\x1b[2J\x1b[3J\x1b[H");
}

auto ConsoleWriteAccess::WriteLine(const ConsoleComponents::MsgType msg_type, std::string_view msg) noexcept -> void { std::println("{}: {}", GetDecoratedMsgTag(msg_type), msg); }

auto ConsoleWriteAccess::Write(const ConsoleComponents::MsgType msg_type, std::string_view msg) noexcept -> void { std::print("{}: {}", GetDecoratedMsgTag(msg_type), msg); }

auto ConsoleWriteAccess::GetDecoratedMsgTag(const ConsoleComponents::MsgType msg_type) noexcept -> std::string { return std::string{std::format("{}{}{}", GetMsgTagColor(msg_type), GetMsgTag(msg_type), "\x1b[0m")}; }

auto ConsoleWriteAccess::GetMsgTag(const ConsoleComponents::MsgType msg_type) noexcept -> std::string_view {
    switch (msg_type) {
        case ConsoleComponents::MsgType::Info:
            return "[Info]";
        case ConsoleComponents::MsgType::Tip:
            return "[Tip]";
        case ConsoleComponents::MsgType::Request:
            return "[Request]";
        case ConsoleComponents::MsgType::Error:
            return "[Error]";
    }

    assert(false && "Unhandled MsgType in Console::GetMsgTag");
    std::terminate();
}

auto ConsoleWriteAccess::GetMsgTagColor(const ConsoleComponents::MsgType msg_type) noexcept -> std::string_view {
    switch (msg_type) {
        case ConsoleComponents::MsgType::Info:
            return "\x1b[90m";
        case ConsoleComponents::MsgType::Tip:
            return "\x1b[32m";
        case ConsoleComponents::MsgType::Request:
            return "\x1b[35m";
        case ConsoleComponents::MsgType::Error:
            return "\x1b[31m";
    }

    assert(false && "Unhandled MsgType in Console::GetMsgTagColor");
    std::terminate();
}