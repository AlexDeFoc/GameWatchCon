module;

#include <iostream>
#include <string>
#include <vector>

export module Core:Console;

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

    template <typename ListOptsFunc>
    auto RequestGameID(ListOptsFunc, std::pair<int, int>) noexcept -> void;

    template <typename ListOptsFunc>
    auto RequestMenuOptionID(ListOptsFunc, std::pair<int, int>, RequestIsCancellable) noexcept -> void;

    Console() noexcept;
    auto RequestGameTitle(RequestIsCancellable = RequestIsCancellable::Yes) noexcept -> void; // TODO: Add more context to the function
    auto WriteLineAndCache(MsgType, std::string_view) noexcept -> void; // TODO: Check usage (may need to remove)
    auto WriteAndCache(MsgType, std::string_view) noexcept -> void; // TODO: Check usage (may need to remove)
    auto WriteCachedMsgs() noexcept -> void;
    auto SetInputRequestStatus(InputRequestStatus) noexcept -> void;
    [[nodiscard]] auto GetInputRequestStatus() const noexcept -> InputRequestStatus;
    [[nodiscard]] auto GetNumberInputResult() const noexcept -> int;
    [[nodiscard]] auto GetStringInputResult() const noexcept -> std::string_view;

    static auto WriteLine(MsgType, std::string_view) noexcept -> void;
    static auto Write(MsgType, std::string_view) noexcept -> void;
    static auto ClearScreen() noexcept -> void;

private:
    template <typename ListOptsFunc, typename RequestMsgFunc>
    auto RequestNumberInput(ListOptsFunc, RequestMsgFunc, std::pair<int, int>, RequestIsCancellable = RequestIsCancellable::Yes) noexcept -> void;

    template <typename RequestMsgFunc>
    auto RequestStringInput(RequestMsgFunc, RequestIsCancellable = RequestIsCancellable::Yes) noexcept -> void;

    [[nodiscard]] static auto GetDecoratedMsgTag(MsgType) noexcept -> std::string;
    [[nodiscard]] static auto GetMsgTag(MsgType) noexcept -> std::string_view;
    [[nodiscard]] static auto GetMsgTagColor(MsgType) noexcept -> std::string_view;

    std::vector<std::string> cached_msgs_;
    InputRequestStatus input_request_status_;
    int number_input_result_;
    std::string string_input_result_;
};

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

        return;
    }

    input_request_status_ = InputRequestStatus::Success;
}

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

        return;
    }

    input_request_status_ = InputRequestStatus::Success;
}
}; // namespace gw::con::core