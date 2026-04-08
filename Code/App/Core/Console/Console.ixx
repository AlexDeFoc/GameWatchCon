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
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

export module Console;

export namespace gw::con::core {
class Console {
public:
    enum class MsgType : int { Info,
                               Tip,
                               Request,
                               Error };

    enum class InputRequestStatus : int { Success,
                                          Cancelled,
                                          Invalid };

    enum class RequestIsCancellable : bool { No,
                                             Yes };

    Console() noexcept;

    ~Console();

    template <typename ListOptsFunc>
    auto RequestGameID(ListOptsFunc, std::pair<int, int>) noexcept -> void;

    template <typename ListOptsFunc>
    auto RequestMenuOptionID(ListOptsFunc, std::pair<int, int>, RequestIsCancellable = RequestIsCancellable::Yes) noexcept -> void;

    static auto RequestKeyPress() noexcept -> void;

    auto ClearScreen() const noexcept -> void;

    auto RequestUserConfirmation() noexcept -> void;

    auto RequestGameTitle(RequestIsCancellable = RequestIsCancellable::Yes) noexcept -> void; // TODO: Add more context to the function

    auto WriteCachedMsgs() noexcept -> void;

    auto WriteLineToCache(MsgType, std::string_view) noexcept -> void;

    auto WriteLine(MsgType, std::string_view) const noexcept -> void;

    auto Write(MsgType, std::string_view) const noexcept -> void;

    [[nodiscard]] auto GetInputRequestStatus() const noexcept -> InputRequestStatus;

    [[nodiscard]] auto GetNumberInputResult() const noexcept -> int;

    [[nodiscard]] auto GetStringInputResult() const noexcept -> std::string_view;

    [[nodiscard]] auto GetUserConfirmationStatus() const noexcept -> bool;

private:
    template <typename RequestMsgFunc>
    auto RequestStringInput(RequestMsgFunc, RequestIsCancellable = RequestIsCancellable::Yes) noexcept -> void;

    template <typename ListOptsFunc, typename RequestMsgFunc>
    auto RequestNumberInput(ListOptsFunc, RequestMsgFunc, std::pair<int, int>, RequestIsCancellable = RequestIsCancellable::Yes) noexcept -> void;

    [[nodiscard]] auto GetDecoratedMsgTag(MsgType) const noexcept -> std::string;

    [[nodiscard]] static auto GetMsgTag(MsgType) noexcept -> std::string_view;

    [[nodiscard]] static auto GetMsgTagColor(MsgType) noexcept -> std::string_view;

    InputRequestStatus input_request_status_;
    int number_input_result_;
    std::string string_input_result_;
    bool user_confirmed_;
    std::vector<std::string> cached_msgs_;

#ifdef _WIN32
    bool input_configured_for_utf8_;
    UINT input_original_con_codepage_;
    UINT input_current_con_codepage_;
    bool output_configured_for_vt_seq_;
    bool output_configured_for_utf8_;
    DWORD output_original_con_mode_;
    DWORD output_current_con_mode_;
    UINT output_original_con_codepage_;
    UINT output_current_con_codepage_;
#endif
};

// Public Member Methods
template <typename ListOptsFunc>
auto Console::RequestGameID(ListOptsFunc list_opts_func, std::pair<int, int> bounds) noexcept -> void {
    static auto request_msg_func = [&] { Write(MsgType::Request, "Enter game id: "); };

    RequestNumberInput(list_opts_func, request_msg_func, std::move(bounds));
}

template <typename ListOptsFunc>
auto Console::RequestMenuOptionID(ListOptsFunc list_opts_func, std::pair<int, int> bounds, RequestIsCancellable request_is_cancellable) noexcept -> void {
    static auto request_msg_func = [&] { Write(MsgType::Request, "Enter option id: "); };

    RequestNumberInput(list_opts_func, request_msg_func, std::move(bounds), request_is_cancellable);
}

// Private Member Methods
template <typename RequestMsgFunc>
auto Console::RequestStringInput(RequestMsgFunc request_msg_func, const RequestIsCancellable request_is_cancellable) noexcept -> void {
    if (request_is_cancellable == RequestIsCancellable::Yes)
        WriteLine(MsgType::Tip, "Enter CTRL+Z to cancel");

    request_msg_func();

    if (std::string input{}; std::getline(std::cin, input)) {
        string_input_result_ = std::move(input);
    } else if (std::cin.eof() || std::cin.fail()) {
        if (request_is_cancellable == RequestIsCancellable::Yes)
            input_request_status_ = InputRequestStatus::Cancelled;
        else
            input_request_status_ = InputRequestStatus::Invalid;

        std::cin.clear();

        return;
    }

    input_request_status_ = InputRequestStatus::Success;
}

template <typename ListOptsFunc, typename RequestMsgFunc>
auto Console::RequestNumberInput(ListOptsFunc list_opts_func, RequestMsgFunc request_msg_func, const std::pair<int, int> bounds, const RequestIsCancellable request_is_cancellable) noexcept -> void {
    list_opts_func();

    if (request_is_cancellable == RequestIsCancellable::Yes)
        WriteLine(MsgType::Tip, "Enter CTRL+Z to cancel");

    request_msg_func();

    if (std::string input{}; std::getline(std::cin, input)) {
        try {
            number_input_result_ = std::stoi(input);

            if (number_input_result_ < bounds.first || number_input_result_ > bounds.second) {
                input_request_status_ = InputRequestStatus::Invalid;
                return;
            }
        } catch (std::invalid_argument&) {
            input_request_status_ = InputRequestStatus::Invalid;
            return;
        } catch (std::out_of_range&) {
            input_request_status_ = InputRequestStatus::Invalid;
            return;
        }
    } else if (std::cin.eof() || std::cin.fail()) {
        if (request_is_cancellable == RequestIsCancellable::Yes)
            input_request_status_ = InputRequestStatus::Cancelled;
        else
            input_request_status_ = InputRequestStatus::Invalid;

        std::cin.clear();

        return;
    }

    input_request_status_ = InputRequestStatus::Success;
}
}; // namespace gw::con::core