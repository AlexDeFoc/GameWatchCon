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
#include <iostream>
#include <print>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

module Console;

using namespace gw::con::core;

#ifdef _WIN32
Console::Console() noexcept : input_request_status_{}, number_input_result_{}, user_confirmed_{false}, input_configured_for_utf8_{false}, input_current_con_codepage_{CP_UTF8}, output_configured_for_vt_seq_{false}, output_configured_for_utf8_{false}, output_current_con_mode_{ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING}, output_current_con_codepage_{CP_UTF8} {
    [&] {
        if ((input_original_con_codepage_ = ::GetConsoleCP()) == 0)
            return;

        if (::SetConsoleCP(input_current_con_codepage_) != 0)
            input_configured_for_utf8_ = true;
    }();

    [&] {
        if ((output_original_con_codepage_ = ::GetConsoleOutputCP()) == 0)
            return;

        if (::SetConsoleOutputCP(output_original_con_codepage_) != 0)
            output_configured_for_utf8_ = true;
    }();

    [&] {
        const auto std_out_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

        if (std_out_handle == INVALID_HANDLE_VALUE)
            return;

        if (::GetConsoleMode(std_out_handle, &output_original_con_mode_) == 0)
            return;

        if (::SetConsoleMode(std_out_handle, output_current_con_mode_) != 0)
            output_configured_for_vt_seq_ = true;
    }();
}
#else
Console::Console() noexcept : input_request_status_{}, number_input_result_{}, user_confirmed_{false} {}
#endif

#ifdef _WIN32
Console::~Console() {
    if (input_configured_for_utf8_)
        ::SetConsoleCP(input_original_con_codepage_);

    if (output_configured_for_utf8_)
        ::SetConsoleOutputCP(output_original_con_codepage_);

    const auto std_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

    if (std_handle == INVALID_HANDLE_VALUE)
        return;

    if (output_configured_for_vt_seq_)
        ::SetConsoleMode(std_handle, output_original_con_mode_);
}
#else
Console::~Console() = default;
#endif

// Public Static Methods
auto Console::RequestKeyPress() noexcept -> void {
    std::string _{};
    std::getline(std::cin, _);
}

// Public Member Methods
auto Console::ClearScreen() const noexcept -> void {
    if (output_configured_for_vt_seq_)
        std::print("\x1b[2J\x1b[3J\x1b[H");
    else
        std::print("\n\n\n\n\n\n\n\n\n\n");
}

auto Console::RequestUserConfirmation() noexcept -> void {
    static auto list_func = [&] {
        std::println("Are you sure?");
        std::println("1. Yes");
        std::println("0. No");
    };

    RequestMenuOptionID(list_func, {0, 1}, RequestIsCancellable::No);

    user_confirmed_ = number_input_result_ == 1;
}

auto Console::RequestGameTitle(const RequestIsCancellable request_is_cancellable) noexcept -> void {
    static auto request_msg_func = [&] { Write(MsgType::Request, "Enter game new title: "); };

    RequestStringInput(request_msg_func, request_is_cancellable);
}

auto Console::WriteCachedMsgs() noexcept -> void {
    for (const auto& msg : cached_msgs_)
        std::print("{}", msg);

    cached_msgs_.clear();
}

auto Console::WriteLineToCache(const MsgType msg_type, std::string_view msg) noexcept -> void { cached_msgs_.push_back(std::format("{}: {}\n", GetDecoratedMsgTag(msg_type), msg)); }

auto Console::WriteLine(const MsgType msg_type, std::string_view msg) const noexcept -> void { std::println("{}: {}", GetDecoratedMsgTag(msg_type), msg); }

auto Console::Write(const MsgType msg_type, std::string_view msg) const noexcept -> void { std::print("{}: {}", GetDecoratedMsgTag(msg_type), msg); }

auto Console::GetInputRequestStatus() const noexcept -> InputRequestStatus { return input_request_status_; }

auto Console::GetNumberInputResult() const noexcept -> int { return number_input_result_; }

auto Console::GetStringInputResult() const noexcept -> std::string_view { return string_input_result_; }

auto Console::GetUserConfirmationStatus() const noexcept -> bool { return user_confirmed_; }

// Private Member Methods
auto Console::GetDecoratedMsgTag(const MsgType msg_type) const noexcept -> std::string {
#ifdef _WIN32
    if (output_configured_for_vt_seq_)
        return std::string{std::format("{}{}{}", GetMsgTagColor(msg_type), GetMsgTag(msg_type), "\x1b[0m")};

    return std::string{GetMsgTag(msg_type)};
#else
    return std::string{std::format("{}{}{}", GetMsgTagColor(msg_type), GetMsgTag(msg_type), "\x1b[0m")};
#endif
}

// Private Static Methods
auto Console::GetMsgTag(const MsgType msg_type) noexcept -> std::string_view {
    switch (msg_type) {
        case MsgType::Info:
            return "[Info]";
        case MsgType::Tip:
            return "[Tip]";
        case MsgType::Request:
            return "[Request]";
        case MsgType::Error:
            return "[Error]";
    }

    assert(false && "Unhandled MsgType in Console::GetMsgTag");
    std::terminate();
}

auto Console::GetMsgTagColor(const MsgType msg_type) noexcept -> std::string_view {
    switch (msg_type) {
        case MsgType::Info:
            return "\x1b[90m";
        case MsgType::Tip:
            return "\x1b[32m";
        case MsgType::Request:
            return "\x1b[35m";
        case MsgType::Error:
            return "\x1b[31m";
    }

    assert(false && "Unhandled MsgType in Console::GetMsgTagColor");
    std::terminate();
}