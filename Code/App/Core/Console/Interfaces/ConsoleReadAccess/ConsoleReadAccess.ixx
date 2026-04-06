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
#include <string>

export module Console:ConsoleReadAccess;

import :ConsoleComponents;
import :ConsoleWriteAccess;

export namespace gw::con::core {
class ConsoleReadAccess {
public:
    template <typename ListOptsFunc>
    auto RequestGameID(ListOptsFunc, std::pair<int, int>) noexcept -> void;

    template <typename ListOptsFunc>
    auto RequestMenuOptionID(ListOptsFunc, std::pair<int, int>, ConsoleComponents::RequestIsCancellable) noexcept -> void;

    ConsoleReadAccess() noexcept;
    auto RequestGameTitle(ConsoleComponents::RequestIsCancellable) noexcept -> void; // TODO: Add more context to the function
    auto SetInputRequestStatus(ConsoleComponents::InputRequestStatus) noexcept -> void;
    [[nodiscard]] auto GetInputRequestStatus() const noexcept -> ConsoleComponents::InputRequestStatus;
    [[nodiscard]] auto GetNumberInputResult() const noexcept -> int;
    [[nodiscard]] auto GetStringInputResult() const noexcept -> std::string_view;

private:
    template <typename RequestMsgFunc>
    auto RequestStringInput(RequestMsgFunc, ConsoleComponents::RequestIsCancellable = ConsoleComponents::RequestIsCancellable::Yes) noexcept -> void;

    template <typename ListOptsFunc, typename RequestMsgFunc>
    auto RequestNumberInput(ListOptsFunc, RequestMsgFunc, std::pair<int, int>, ConsoleComponents::RequestIsCancellable = ConsoleComponents::RequestIsCancellable::Yes) noexcept -> void;

    ConsoleComponents::InputRequestStatus input_request_status_;
    int number_input_result_;
    std::string string_input_result_;
};

template <typename RequestMsgFunc>
auto ConsoleReadAccess::RequestStringInput(RequestMsgFunc request_msg_func, const ConsoleComponents::RequestIsCancellable request_is_cancellable) noexcept -> void {
    if (request_is_cancellable == ConsoleComponents::RequestIsCancellable::Yes)
        ConsoleWriteAccess::WriteLine(ConsoleComponents::MsgType::Tip, "Enter CTRL+Z to cancel");

    request_msg_func();

    if (std::string input{}; std::getline(std::cin, input)) {
        string_input_result_ = std::move(input);
    } else if (std::cin.eof() || std::cin.fail()) {
        if (request_is_cancellable == ConsoleComponents::RequestIsCancellable::Yes)
            input_request_status_ = ConsoleComponents::InputRequestStatus::Cancelled;
        else
            input_request_status_ = ConsoleComponents::InputRequestStatus::Invalid;

        return;
    }

    input_request_status_ = ConsoleComponents::InputRequestStatus::Success;
}

template <typename ListOptsFunc>
auto ConsoleReadAccess::RequestGameID(ListOptsFunc list_opts_func, std::pair<int, int> bounds) noexcept -> void {
    static auto request_msg_func = [&] { ConsoleWriteAccess::Write(ConsoleComponents::MsgType::Request, "Enter game id: "); };

    RequestNumberInput(list_opts_func, request_msg_func, std::move(bounds));
}

template <typename ListOptsFunc>
auto ConsoleReadAccess::RequestMenuOptionID(ListOptsFunc list_opts_func, std::pair<int, int> bounds, ConsoleComponents::RequestIsCancellable request_is_cancellable) noexcept -> void {
    static auto request_msg_func = [&] { ConsoleWriteAccess::Write(ConsoleComponents::MsgType::Request, "Enter option id: "); };

    RequestNumberInput(list_opts_func, request_msg_func, std::move(bounds), request_is_cancellable);
}

template <typename ListOptsFunc, typename RequestMsgFunc>
auto ConsoleReadAccess::RequestNumberInput(ListOptsFunc list_opts_func, RequestMsgFunc request_msg_func, const std::pair<int, int> bounds, const ConsoleComponents::RequestIsCancellable request_is_cancellable) noexcept -> void {
    list_opts_func();

    if (request_is_cancellable == ConsoleComponents::RequestIsCancellable::Yes)
        ConsoleWriteAccess::WriteLine(ConsoleComponents::MsgType::Tip, "Enter CTRL+Z to cancel");

    request_msg_func();

    if (std::string input{}; std::getline(std::cin, input)) {
        try {
            number_input_result_ = std::stoi(input);

            if (number_input_result_ < bounds.first || number_input_result_ > bounds.second) {
                input_request_status_ = ConsoleComponents::InputRequestStatus::Invalid;
                return;
            }
        } catch (std::invalid_argument&) {
            input_request_status_ = ConsoleComponents::InputRequestStatus::Invalid;
            return;
        } catch (std::out_of_range&) {
            input_request_status_ = ConsoleComponents::InputRequestStatus::Invalid;
            return;
        }
    } else if (std::cin.eof() || std::cin.fail()) {
        if (request_is_cancellable == ConsoleComponents::RequestIsCancellable::Yes)
            input_request_status_ = ConsoleComponents::InputRequestStatus::Cancelled;
        else
            input_request_status_ = ConsoleComponents::InputRequestStatus::Invalid;

        return;
    }

    input_request_status_ = ConsoleComponents::InputRequestStatus::Success;
}
} // namespace gw::con::core