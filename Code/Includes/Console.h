// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <string>
#include <array>
#include <iostream>
#include <utility>
#include <format>
#include <source_location>
#include <ranges>

#ifdef _WIN32
using UINT = unsigned int;
using DWORD = unsigned long;
#endif

namespace gw {
class Console {
public:
    // clang-format off
    enum class Tag { Info, Error, Request, Tip, Success, FatalError };
    enum class Color { Red, Green, White, DarkGray, Magenta, Cyan };
    enum class InputStatus { Success, Cancelled };
    // clang-format on

    /*!
     * @brief Builds Console instance
     */
    Console() noexcept;

    /*!
     * @brief Destructs Console instance
     */
    ~Console();

    // TODO: add comment
    [[nodiscard]] auto RequestUserGameIDChoice(std::string, std::int64_t, bool = true, std::int64_t = 0, bool = true) -> std::pair<std::int64_t, InputStatus>;

    /*!
     * @brief Requests from the user to choose Yes or No
     */
    [[nodiscard]] auto RequestUserConfirmation() -> std::pair<bool, InputStatus>;

    /*!
     * @brief Requests from the user a new auto save interval amount in minutes
     * @param formatted_current_interval Formatted string representation of the current interval value
     * @param is_request_cancellable Statement whether the request cancellable
     * @return Interval in minutes & Input Status pair
     */
    [[nodiscard]] auto RequestNewAutoSaveInterval(std::string, bool = true) -> std::pair<std::chrono::minutes, InputStatus>;

    /*!
     * @brief Requests from the user a game title
     * @param is_request_cancellable Statement whether the request cancellable
     * @return Game title in the form of a string
     */
    [[nodiscard]] auto RequestGameTitle(bool = true) -> std::pair<std::string, InputStatus>;

    /*!
     * @brief Colors text
     * @param txt_color Color which to color the text
     * @param txt The text which to color
     * @return Colored text
     */
    [[nodiscard]] auto ColorText(Color, std::string) const -> std::string;

    /*!
     * @brief Logs bug location and requests user to press any key, after which exit app
     * @param bug_location Location of the bug, its provided automatically
     * @param additional_ctx (Optional) Message which would help debug the issue easier
     */
    [[noreturn]] auto ThrowOnUnhandledCase(std::string_view = {}, std::source_location = std::source_location::current()) const -> void;

    /*!
     * @brief Logs bug location and requests user to press any key, after which exit app
     * @param additional_ctx (Optional) Message which would help debug the issue easier
     * @param bug_location Location of the bug, provided manually
     * @param call_site Location of the call site, provided automatically
     */
    [[noreturn]] auto ThrowOnUnhandledCase(std::source_location, std::string_view = {}, std::source_location = std::source_location::current()) const -> void;

    /*!
     * @brief Requests from user to press any key, after which it returns
     */
    static auto RequestKeyPress() noexcept -> void;

    /*!
     * @brief Clears stdout by either printing vt sequences or 10 new lines
     */
    auto ClearCout() const noexcept -> void;

    /*!
     * @brief Writes cached messages into stdout
     */
    auto WriteCachedMsgs() noexcept -> void;

    /*!
     * @brief Writes message into stdout
     * @param msg Message which to write
     */
    static auto Write(std::string_view) noexcept -> void;

    /*!
     * @brief Tags message and writes it into stdout
     * @param tag Message tag
     * @param msg Message which to write
     */
    auto Write(Tag, std::string_view) const -> void;

    /*!
     * @brief Colors message and writes it into stdout
     * @param color Message color
     * @param msg Message which to write
     */
    auto Write(Color, std::string_view) const -> void;

    /*!
     * @brief Adds message to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param msg Message which to add
     */
    auto WriteToCache(std::string_view) noexcept -> void;

    /*!
     * @brief Tags message and adds it to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param tag Message tag
     * @param msg Message which to add
     */
    auto WriteToCache(Tag, std::string_view) -> void;

    /*!
     * @brief Colors message and adds it to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param color Message color
     * @param msg Message which to add
     */
    auto WriteToCache(Color, std::string_view) -> void;

    /*!
     * @brief Writes message into stdout with a new line
     * @param msg Message which to write
     */
    static auto WriteLine(std::string_view) noexcept -> void;

    /*!
     * @brief Tags message and writes it to stdout with a new line
     * @param tag Message tag
     * @param msg Message which to write
     */
    auto WriteLine(Tag, std::string_view) const -> void;

    /*!
     * @brief Colors message and writes it to stdout with a new line
     * @param color Message color
     * @param msg Message which to write
     */
    auto WriteLine(Color, std::string_view) const -> void;

    /*!
     * @brief Adds message with a new line added to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param msg Message which to add
     */
    auto WriteLineToCache(std::string_view) noexcept -> void;

    /*!
     * @brief Tags message and adds it to cache with a new line added to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param tag Message tag
     * @param msg Message which to add
     */
    auto WriteLineToCache(Tag, std::string_view) -> void;

    /*!
     * @brief Colors message and adds it to cache with a new line added to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param color Message color
     * @param msg Message which to add
     */
    auto WriteLineToCache(Color, std::string_view) -> void;

    // TODO: Add concept for the lambda
    /*!
     * @brief Requests from the user to choose a game id from a list printed in cout
     * @param list_games_func Function which lists games for the user to choose the game id from
     * @param games_count The amount of games listed in cout
     * @param is_active_game_id_valid_choice Statement whether the active game id is a valid choice or considered invalid
     * @param active_game_id Active game id, used only when is_active_game_id_valid_choice is true
     * @param is_request_cancellable Statement whether the request is cancellable
     */
    template <typename ListingFunc>
    [[nodiscard]] auto RequestUserGameIDChoice(ListingFunc, int, bool = true, int = 0, bool = true) -> std::pair<int, InputStatus>;

    /*!
     * @brief Lists menu options then requests user input
     * @param menu_opts Menu options which to list to the user
     * @param is_request_cancellable Statement whether the request is cancellable; default = true
     * @param menu_opts_contain_special_index Should the user be allowed to choose the a special index, possibly outside the menu opts index range; default = true
     * @param special_index_val Value of the special index; default = 0
     * @return Index selected and InputStatus pair
     */
    template <std::size_t ArrSize>
    [[nodiscard]] auto RequestUserMenuChoice(std::array<std::string, ArrSize>, bool = true, bool = true, std::size_t = 0) -> std::pair<std::size_t, InputStatus>;

    /*!
     * @brief Logs bug location and requests user to press any key, after which exit app
     * @param bug_location Location of the bug
     * @param fmt Message format
     * @param args... Arguments which go in message
     */
    template <typename... Args>
    auto ThrowOnUnhandledCase(std::source_location, std::format_string<Args...>, Args...) -> void;

    /*!
     * @brief Writes formatted message to stdout
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    static auto Write(std::format_string<Args...>, Args&&...) noexcept -> void;

    /*!
     * @brief Tags formatted message and writes it to stdout
     * @param tag Message tag
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto Write(Tag, std::format_string<Args...>, Args&&...) const -> void;

    /*!
     * @brief Colors formatted message and writes it to stdout
     * @param color Message color
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto Write(Color, std::format_string<Args...>, Args&&...) const -> void;

    /*!
     * @brief Adds formated message to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteToCache(std::format_string<Args...>, Args&&...) noexcept -> void;

    /*!
     * @brief Tags formatted message and adds it to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param tag Message tag
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteToCache(Tag, std::format_string<Args...>, Args&&...) -> void;

    /*!
     * @brief Colors formatted message and adds it to cache, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param color Message color
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteToCache(Color, std::format_string<Args...>, Args&&...) -> void;

    /*!
     * @brief Writes formatted message to stdout with a new line
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    static auto WriteLine(std::format_string<Args...>, Args&&...) noexcept -> void;

    /*!
     * @brief Tags formatted message and writes it to stdout with a new line
     * @param tag Message tag
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteLine(Tag, std::format_string<Args...>, Args&&...) const -> void;

    /*!
     * @brief Color formatted message and writes it to stdout with a new line
     * @param color Message color
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteLine(Color, std::format_string<Args...>, Args&&...) const -> void;

    /*!
     * @brief Adds formated message to cache with a new line, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteLineToCache(std::format_string<Args...>, Args&&...) noexcept -> void;

    /*!
     * @brief Tags formatted message and adds it to cache with a new line, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param tag Message tag
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteLineToCache(Tag, std::format_string<Args...>, Args&&...) -> void;

    /*!
     * @brief Tags formatted message and adds it to cache with a new line, which later will be written to stdout with a call to WriteCachedMsgs()
     * @param tag Message tag
     * @param fmt Formatting string, tells how should the message be formatted from args
     * @param args Arguments which to format into message
     */
    template <typename... Args>
    auto WriteLineToCache(Color, std::format_string<Args...>, Args&&...) -> void;


private:
    [[nodiscard]] auto CheckSupportForANSIEscapes() const noexcept -> bool;

    /*!
     * @brief Gets message color from a message tag
     * @param tag Message tag
     * @return Message color
     */
    [[nodiscard]] auto GetTagAsColor(gw::Console::Tag) const -> gw::Console::Color;

    /*!
     * @brief Gets text representation out of a message tag
     * @param tag Message tag
     * @return text
     */
    [[nodiscard]] auto GetTagAsText(gw::Console::Tag) const -> std::string_view;

    /*!
     * @brief Gets text representation out of a message color
     * @param color Message color
     * @return text
     */
    [[nodiscard]] auto GetColorAsText(gw::Console::Color) const -> std::string_view;

#ifdef _WIN32
    bool cin_utf8_ready{false};
    bool cout_utf8_ready{false};
    UINT cin_codepage_original{};
    UINT cout_codepage_original{};
    DWORD cout_mode_original{};
#endif

    bool cout_supports_colored_text_{false};
    std::string cached_msgs_{};
};
} // namespace gw

// TODO: Check if it even has anymore usage!
template <typename ListingFunc>
auto gw::Console::RequestUserGameIDChoice(ListingFunc list_games_func, int games_count, bool is_active_game_id_valid_choice, int active_game_id, bool is_request_cancellable) -> std::pair<int, InputStatus> {
    while (true) {
        ClearCout();
        list_games_func();
        WriteCachedMsgs();

        if (is_request_cancellable)
            WriteLine(Tag::Tip, "Enter CTRL+Z to cancel");

        Write(Tag::Request, "Enter game id: ");

        if (std::string input{}; std::getline(std::cin, input)) {
            try {
                auto selected_game_id = std::stoi(input);

                if (selected_game_id < 1 || selected_game_id > games_count) {
                    WriteLineToCache(Tag::Error, "Input out of range!");
                    continue;
                } else if (!is_active_game_id_valid_choice && selected_game_id == active_game_id) {
                    WriteLineToCache(Tag::Error, "Cannot select active game! Stop it first!");
                    continue;
                } else {
                    return {selected_game_id, InputStatus::Success};
                }
            } catch (const std::invalid_argument&) {
                WriteLineToCache(Tag::Error, "Invalid input!");
                continue;
            } catch (const std::out_of_range&) {
                WriteLineToCache(Tag::Error, "Input is out of range!");
                continue;
            } catch (...) {
                ThrowOnUnhandledCase();
            }
        } else if (std::cin.eof() || std::cin.fail()) {
            std::cin.clear();
            if (is_request_cancellable) {
                return {-1, InputStatus::Cancelled};
            } else {
                WriteLineToCache(Tag::Error, "Invalid input!");
                continue;
            }
        }
    }
}

template <std::size_t ArrSize>
[[nodiscard]] auto gw::Console::RequestUserMenuChoice(std::array<std::string, ArrSize> menu_opts, bool is_request_cancellable, bool menu_opts_contain_special_index, std::size_t special_index_val) -> std::pair<std::size_t, InputStatus> {
    while (true) {
        ClearCout();
        WriteCachedMsgs();

        auto menu_body = menu_opts | std::ranges::views::take(ArrSize - 1);

        for (auto [i, opt] : menu_body | std::ranges::views::enumerate)
            std::println("{}. {}", i + 1, opt);

        std::println("{}. {}", special_index_val, menu_opts[ArrSize - 1]);

        if (is_request_cancellable)
            WriteLine(Tag::Tip, "Enter CTRL+Z to cancel");

        Write(Tag::Request, "Enter menu option index: ");

        if (std::string input{}; std::getline(std::cin, input)) {
            try {
                std::size_t selected_opt_index = std::stoull(input);

                if ((selected_opt_index < 1 || selected_opt_index > ArrSize - 1) && (menu_opts_contain_special_index && special_index_val != selected_opt_index)) {
                    WriteLineToCache(Tag::Error, "Input out of range!");
                    continue;
                } else
                    return {selected_opt_index, InputStatus::Success};
            } catch (const std::invalid_argument&) {
                WriteLineToCache(Tag::Error, "Invalid input!");
                continue;
            } catch (const std::out_of_range&) {
                WriteLineToCache(Tag::Error, "Input is out of range!");
                continue;
            } catch (...) {
                ThrowOnUnhandledCase();
            }
        } else if (std::cin.eof() || std::cin.fail()) {
            std::cin.clear();
            if (is_request_cancellable) {
                return {-1, InputStatus::Cancelled};
            } else {
                WriteLineToCache(Tag::Error, "Invalid input!");
                continue;
            }
        }
    }
}

template <typename... Args>
auto gw::Console::ThrowOnUnhandledCase(std::source_location bug_location, std::format_string<Args...> fmt, Args... args) -> void {
    WriteLine(gw::Console::Tag::FatalError, "Unhandled case with bug location: File: '{}', Line: '{}', Function: '{}'", bug_location.file_name(), bug_location.line(), bug_location.function_name());
    WriteLine(gw::Console::Tag::Tip, fmt, std::forward<Args>(args)...);
    WriteLine(gw::Console::Tag::Info, "The app will now exit, press any key to proceed.");
    RequestKeyPress();
    throw std::logic_error("Unhandled case encountered in " + std::string(bug_location.function_name()));
}

template <typename... Args>
auto gw::Console::Write(std::format_string<Args...> fmt, Args&&... args) noexcept -> void {
    std::print(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
auto gw::Console::Write(const Tag tag, std::format_string<Args...> fmt, Args&&... args) const -> void {
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::print("{}{}{}: {}", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::print("{}: {}", GetTagAsText(tag), msg);
}

template <typename... Args>
auto gw::Console::Write(const Color msg_color, std::format_string<Args...> fmt, Args&&... args) const -> void {
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::print("{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::print("{}", msg);
}

template <typename... Args>
auto gw::Console::WriteToCache(std::format_string<Args...> fmt, Args&&... args) noexcept -> void {
    std::format_to(std::back_inserter(cached_msgs_), fmt, std::forward<Args>(args)...);
}

template <typename... Args>
auto gw::Console::WriteToCache(Tag tag, std::format_string<Args...> fmt, Args&&... args) -> void {
    auto it = std::back_inserter(cached_msgs_);
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}: {}", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::format_to(it, "{}: {}", GetTagAsText(tag), msg);
}

template <typename... Args>
auto gw::Console::WriteToCache(const Color msg_color, std::format_string<Args...> fmt, Args&&... args) -> void {
    auto it = std::back_inserter(cached_msgs_);
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::format_to(it, "{}", msg);
}

template <typename... Args>
auto gw::Console::WriteLine(std::format_string<Args...> fmt, Args&&... args) noexcept -> void {
    std::println(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
auto gw::Console::WriteLine(const Tag tag, std::format_string<Args...> fmt, Args&&... args) const -> void {
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::println("{}{}{}: {}", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::println("{}: {}", GetTagAsText(tag), msg);
}

template <typename... Args>
auto gw::Console::WriteLine(const Color msg_color, std::format_string<Args...> fmt, Args&&... args) const -> void {
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::println("{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::println("{}", msg);
}

template <typename... Args>
auto gw::Console::WriteLineToCache(std::format_string<Args...> fmt, Args&&... args) noexcept -> void {
    std::format_to(std::back_inserter(cached_msgs_), fmt, std::forward<Args>(args)...);
    cached_msgs_.push_back('\n');
}

template <typename... Args>
auto gw::Console::WriteLineToCache(const Tag tag, std::format_string<Args...> fmt, Args&&... args) -> void {
    auto it = std::back_inserter(cached_msgs_);
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}: {}\n", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::format_to(it, "{}: {}\n", GetTagAsText(tag), msg);
}

template <typename... Args>
auto gw::Console::WriteLineToCache(const Color msg_color, std::format_string<Args...> fmt, Args&&... args) -> void {
    auto it = std::back_inserter(cached_msgs_);
    auto msg = std::format(fmt, std::forward<Args>(args)...);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::format_to(it, "{}", msg);
}
