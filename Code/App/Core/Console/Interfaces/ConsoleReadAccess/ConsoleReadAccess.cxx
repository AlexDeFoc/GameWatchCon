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

#include <iostream>
#include <print>
#include <string>

module Console;
import :ConsoleReadAccess;

using namespace gw::con::core;

ConsoleReadAccess::ConsoleReadAccess() noexcept : input_request_status_{}, number_input_result_{}, user_confirmed_{false} {}

auto ConsoleReadAccess::RequestUserConfirmation() noexcept -> void {
    static auto list_func = [&] {
        std::println("Are you sure?");
        std::println("1. Yes");
        std::println("0. No");
    };

    RequestMenuOptionID(list_func, {0, 1}, ConsoleComponents::RequestIsCancellable::No);
}

auto ConsoleReadAccess::RequestGameTitle(const ConsoleComponents::RequestIsCancellable request_is_cancellable) noexcept -> void {
    static auto request_msg_func = [&] { ConsoleWriteAccess::Write(ConsoleComponents::MsgType::Request, "Enter game new title: "); };

    RequestStringInput(request_msg_func, request_is_cancellable);
}

auto ConsoleReadAccess::SetInputRequestStatus(const ConsoleComponents::InputRequestStatus new_status) noexcept -> void { input_request_status_ = new_status; }

auto ConsoleReadAccess::GetInputRequestStatus() const noexcept -> ConsoleComponents::InputRequestStatus { return input_request_status_; }

auto ConsoleReadAccess::GetNumberInputResult() const noexcept -> int { return number_input_result_; }

auto ConsoleReadAccess::GetStringInputResult() const noexcept -> std::string_view { return string_input_result_; }

auto ConsoleReadAccess::GetUserConfirmationStatus() const noexcept -> bool { return user_confirmed_; }

auto ConsoleReadAccess::RequestKeyPress() noexcept -> void {
    std::string _{};
    std::getline(std::cin, _);
}