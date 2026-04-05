module;

#include <cassert>
#include <print>

module Core;
import :Console;

using namespace gw::con::core;

Console::Console() noexcept : input_request_status_{}, number_input_result_{}, string_input_result_{} {}

auto Console::RequestGameTitle(const RequestIsCancellable request_is_cancellable) noexcept -> void {
    static auto request_msg_func = [&] { Write(MsgType::Request, "Enter game new title: "); };

    RequestStringInput(request_msg_func, request_is_cancellable);
}

auto Console::WriteLineAndCache(const MsgType msg_type, std::string_view msg) noexcept -> void {
    auto full_msg = std::format("{}: {}\n", GetDecoratedMsgTag(msg_type), msg);
    cached_msgs_.push_back(full_msg);
    std::print("{}", full_msg);
}

auto Console::WriteAndCache(const MsgType msg_type, std::string_view msg) noexcept -> void {
    auto full_msg = std::format("{}: {}", GetDecoratedMsgTag(msg_type), msg);
    cached_msgs_.push_back(full_msg);
    std::print("{}", full_msg);
}

auto Console::WriteCachedMsgs() noexcept -> void {
    for (const auto& msg : cached_msgs_)
        std::print("{}", msg);

    cached_msgs_.clear();
}

auto Console::SetInputRequestStatus(const InputRequestStatus new_status) noexcept -> void { input_request_status_ = new_status; }

auto Console::GetInputRequestStatus() const noexcept -> InputRequestStatus { return input_request_status_; }

auto Console::GetNumberInputResult() const noexcept -> int { return number_input_result_; }

auto Console::GetStringInputResult() const noexcept -> std::string_view { return string_input_result_; }

// Static Methods
auto Console::ClearScreen() noexcept -> void { std::print("\x1b[2J\x1b[H"); }

auto Console::WriteLine(const MsgType msg_type, std::string_view msg) noexcept -> void { std::println("{}: {}", GetDecoratedMsgTag(msg_type), msg); }

auto Console::Write(const MsgType msg_type, std::string_view msg) noexcept -> void { std::print("{}: {}", GetDecoratedMsgTag(msg_type), msg); }

auto Console::GetDecoratedMsgTag(const MsgType msg_type) noexcept -> std::string { return std::string{std::format("{}{}{}", GetMsgTagColor(msg_type), GetMsgTag(msg_type), "\x1b[0m")}; }

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
            return "\x1b[37m";
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