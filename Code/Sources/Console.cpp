// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Console.hpp"

// Public
gw::Console::Console() noexcept {
#ifdef _WIN32
    [&] {
        if ((cin_codepage_original = ::GetConsoleCP()) == 0)
            return;

        if (::SetConsoleCP(CP_UTF8) != 0)
            cin_utf8_ready = true;
    }();

    [&] {
        if ((cout_codepage_original = ::GetConsoleOutputCP()) == 0)
            return;

        if (::SetConsoleOutputCP(CP_UTF8) != 0)
            cout_utf8_ready = true;
    }();

    [&] {
        const auto std_out_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

        if (std_out_handle == INVALID_HANDLE_VALUE)
            return;

        if (::GetConsoleMode(std_out_handle, &cout_mode_original) == 0)
            return;

        if (::SetConsoleMode(std_out_handle, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0)
            cout_supports_colored_text_ = true;
    }();
#else
    cout_supports_colored_text_ = CheckSupportForANSIEscapes();
#endif
}

gw::Console::~Console() {
#ifdef _WIN32
    if (cin_utf8_ready)
        ::SetConsoleCP(cin_codepage_original);

    if (cout_utf8_ready)
        ::SetConsoleOutputCP(cout_codepage_original);

    const auto cout_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

    if (cout_handle == INVALID_HANDLE_VALUE)
        return;

    if (cout_supports_colored_text_)
        ::SetConsoleMode(cout_handle, cout_mode_original);
#endif
}

auto gw::Console::RequestUserGameIDChoice(std::string printable_games, std::int64_t games_count, bool is_active_game_id_valid_choice, std::int64_t active_game_id, bool is_request_cancellable) -> std::pair<std::int64_t, InputStatus> {
    while (true) {
        ClearCout();
        std::println("{}", printable_games);
        WriteCachedMsgs();

        if (is_request_cancellable)
            WriteLine(Tag::Tip, "Enter CTRL+Z to cancel");

        Write(Tag::Request, "Enter game id: ");

        if (std::string input{}; std::getline(std::cin, input)) {
            try {
                std::int64_t selected_game_id = std::stoll(input);

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

auto gw::Console::RequestUserConfirmation() -> std::pair<bool, InputStatus> {
    while (true) {
        ClearCout();
        WriteCachedMsgs();

        std::puts("Are you sure");
        std::puts("1. Yes");
        std::puts("0. No");
        Write(Tag::Request, "Enter option index: ");

        if (std::string input{}; std::getline(std::cin, input)) {
            try {
                const std::int64_t selected_opt_index = std::stoll(input);

                if (selected_opt_index != 1 && selected_opt_index != 0) {
                    WriteLineToCache(Tag::Error, "Input out of range!");
                    continue;
                } else
                    return {selected_opt_index == 1, InputStatus::Success};
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
            WriteLineToCache(Tag::Error, "Invalid input!");
            continue;
        }
    }
}

auto gw::Console::RequestNewAutoSaveInterval(std::string formatted_current_interval, const bool is_request_cancellable) -> std::pair<std::chrono::minutes, InputStatus> {
    while (true) {
        ClearCout();
        WriteCachedMsgs();

        WriteLine(Console::Tag::Info, "Current inverval value: {}", formatted_current_interval);

        if (is_request_cancellable)
            WriteLine(Tag::Tip, "Enter CTRL+Z to cancel");

        Write(Tag::Request, "Enter new interval (in minutes): ");

        if (std::string input{}; std::getline(std::cin, input)) {
            try {
                std::int64_t new_interval = std::stoll(input);

                if (new_interval < 1) {
                    WriteLineToCache(Tag::Error, "Interval needs to be at least 1 minute!");
                    continue;
                } else
                    return {std::chrono::minutes(new_interval), InputStatus::Success};
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
                return {std::chrono::minutes::zero(), InputStatus::Cancelled};
            } else {
                WriteLineToCache(Tag::Error, "Invalid input!");
                continue;
            }
        }
    }
}

auto gw::Console::RequestGameTitle(const bool is_request_cancellable) -> std::pair<std::string, InputStatus> {
    while (true) {
        ClearCout();
        WriteCachedMsgs();

        if (is_request_cancellable)
            WriteLine(Tag::Tip, "Enter CTRL+Z to cancel");

        Write(Tag::Request, "Enter new game title: ");

        if (std::string input{}; std::getline(std::cin, input)) {
            return {std::move(input), InputStatus::Success};
        } else if (std::cin.eof() || std::cin.fail()) {
            std::cin.clear();
            if (is_request_cancellable) {
                return {{}, InputStatus::Cancelled};
            } else {
                WriteLineToCache(Tag::Error, "Invalid input!");
                continue;
            }
        }
    }
}

auto gw::Console::ColorText(const Color txt_color, std::string txt) const -> std::string {
    if (cout_supports_colored_text_)
        return std::format("{}{}{}", GetColorAsText(txt_color), std::move(txt), "\x1b[0m");
    else
        return txt;
}

auto gw::Console::ThrowOnUnhandledCase(const std::string_view additional_ctx, const std::source_location bug_location) const -> void {
    WriteLine(gw::Console::Tag::FatalError, "Unhandled case with bug location: File: '{}', Line: '{}', Function: '{}'", bug_location.file_name(), bug_location.line(), bug_location.function_name());
    if (!additional_ctx.empty())
        WriteLine(gw::Console::Tag::Tip, "Additional context - {}", additional_ctx);
    WriteLine(gw::Console::Tag::Info, "The app will now exit, press any key to proceed.");
    RequestKeyPress();
    throw std::logic_error({});
}

auto gw::Console::ThrowOnUnhandledCase(const std::source_location bug_location, const std::string_view additional_ctx, const std::source_location call_site) const -> void {
    WriteLine(gw::Console::Tag::FatalError, "Unhandled case with bug location at: File: '{}', Line: '{}', Function: '{}'; With call site at: File: '{}', Line: '{}', Function: '{}'", bug_location.file_name(), bug_location.line(), bug_location.function_name(), call_site.file_name(), call_site.line(), call_site.function_name());
    if (!additional_ctx.empty())
        WriteLine(gw::Console::Tag::Tip, "Additional context - {}", additional_ctx);
    WriteLine(gw::Console::Tag::Info, "The app will now exit, press any key to proceed.");
    RequestKeyPress();
    throw std::logic_error("Unhandled case encountered in " + std::string(bug_location.function_name()));
}

auto gw::Console::RequestKeyPress() noexcept -> void {
#ifdef _WIN32
    bool failed{false};
    [&] {
        const auto std_in_handle = ::GetStdHandle(STD_INPUT_HANDLE);
        DWORD cin_mode_original{};

        if (std_in_handle == INVALID_HANDLE_VALUE) {
            failed = true;
            return;
        }

        if (::GetConsoleMode(std_in_handle, &cin_mode_original) == 0) {
            failed = true;
            return;
        }

        if (::SetConsoleMode(std_in_handle, cin_mode_original & (~(static_cast<DWORD>(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)))) == 0) {
            failed = true;
            return;
        }

        INPUT_RECORD ir{};
        DWORD events{};

        do {
            ReadConsoleInputW(std_in_handle, &ir, 1, &events);
        } while (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown);

        ::SetConsoleMode(std_in_handle, cin_mode_original);
    }();

    if (failed) {
        std::string dummy_input;
        std::getline(std::cin, dummy_input);
    }

#else
    // RAII guard for terminal settings
    struct termios_guard {
        int fd;
        termios original;
        bool valid;

        termios_guard() noexcept : fd{STDIN_FILENO}, valid{false} {
            if (::tcgetattr(fd, &original) == 0) {
                termios raw = original;
                raw.c_lflag &= ~static_cast<tcflag_t>(ECHO | ICANON);
                raw.c_cc[VMIN] = 1;
                raw.c_cc[VTIME] = 0;
                if (::tcsetattr(fd, TCSAFLUSH, &raw) == 0) {
                    valid = true;
                    return;
                }
            }
        }
        ~termios_guard() noexcept {
            if (valid) {
                ::tcsetattr(fd, TCSAFLUSH, &original);
            }
        }
    } guard;

    if (!guard.valid) {
        std::string dummy;
        std::getline(std::cin, dummy);
        return;
    }

    char ch;
    while (::read(STDIN_FILENO, &ch, 1) == -1 && errno == EINTR)
        ;
#endif
}

auto gw::Console::ClearCout() const noexcept -> void {
    if (cout_supports_colored_text_)
        std::print("\x1b[2J\x1b[3J\x1b[H");
    else
        std::print("\n\n\n\n\n\n\n\n\n\n");
}

auto gw::Console::WriteCachedMsgs() noexcept -> void {
    std::print("{}", cached_msgs_);
    cached_msgs_.clear();
}

auto gw::Console::Write(const std::string_view msg) noexcept -> void {
    std::print("{}", msg);
}

auto gw::Console::Write(const Tag tag, const std::string_view msg) const -> void {
    if (cout_supports_colored_text_)
        std::print("{}{}{}: {}", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::print("{}: {}", GetTagAsText(tag), msg);
}

auto gw::Console::Write(const Color msg_color, const std::string_view msg) const -> void {
    if (cout_supports_colored_text_)
        std::print("{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::print("{}", msg);
}

auto gw::Console::WriteToCache(const std::string_view msg) noexcept -> void {
    cached_msgs_.append(msg);
}

auto gw::Console::WriteToCache(const Tag tag, const std::string_view msg) -> void {
    auto it = std::back_inserter(cached_msgs_);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}: {}", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::format_to(it, "{}: {}", GetTagAsText(tag), msg);
}

auto gw::Console::WriteToCache(const Color msg_color, const std::string_view msg) -> void {
    auto it = std::back_inserter(cached_msgs_);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::format_to(it, "{}", msg);
}

auto gw::Console::WriteLine(std::string msg) noexcept -> void {
    std::println("{}", std::move(msg));
}

auto gw::Console::WriteLine(const std::string_view msg) noexcept -> void {
    std::println("{}", msg);
}

auto gw::Console::WriteLine(const Tag tag, const std::string_view msg) const -> void {
    if (cout_supports_colored_text_)
        std::println("{}{}{}: {}", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::println("{}: {}", GetTagAsText(tag), msg);
}

auto gw::Console::WriteLine(const Color msg_color, const std::string_view msg) const -> void {
    if (cout_supports_colored_text_)
        std::println("{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::println("{}", msg);
}

auto gw::Console::WriteLineToCache(const std::string_view msg) noexcept -> void {
    cached_msgs_.append(msg);
    cached_msgs_.push_back('\n');
}

auto gw::Console::WriteLineToCache(const Tag tag, const std::string_view msg) -> void {
    auto it = std::back_inserter(cached_msgs_);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}: {}\n", GetColorAsText(GetTagAsColor(tag)), GetTagAsText(tag), "\x1b[0m", msg);
    else
        std::format_to(it, "{}: {}\n", GetTagAsText(tag), msg);
}

auto gw::Console::WriteLineToCache(const Color msg_color, const std::string_view msg) -> void {
    auto it = std::back_inserter(cached_msgs_);

    if (cout_supports_colored_text_)
        std::format_to(it, "{}{}{}", GetColorAsText(msg_color), msg, "\x1b[0m");
    else
        std::format_to(it, "{}", msg);
}

// Private
auto gw::Console::CheckSupportForANSIEscapes() const noexcept -> bool {
#ifdef _WIN32
    return cout_supports_colored_text_;
#else
    // 1. Must be a real terminal
    if (!isatty(STDOUT_FILENO))
        return false;

    // 2. Respect NO_COLOR convention (https://no-color.org/)
    if (const char* no_color = std::getenv("NO_COLOR"))
        if (no_color[0] != '\0') // variable exists and is non-empty
            return false;

    // 3. Check TERM (ignore case for safety)
    const char* term = std::getenv("TERM");
    if (term == nullptr || term[0] == '\0')
        return false;
    if (std::strcmp(term, "dumb") == 0)
        return false;

    // All checks passed – assume 16‑colour ANSI support.
    return true;
#endif
}

auto gw::Console::GetTagAsColor(const Tag tag_type) const -> gw::Console::Color {
    switch (tag_type) {
        case Tag::Tip:
            return Color::DarkGray;
        case Tag::Request:
            return Color::Magenta;
        case Tag::Error:
            return Color::Red;
        case Tag::Success:
            return Color::Green;
        case Tag::Info:
            return Color::Cyan;
        case Tag::FatalError:
            return Color::Red;
        default:
            ThrowOnUnhandledCase();
    }
}

auto gw::Console::GetTagAsText(const Tag tag_type) const -> std::string_view {
    switch (tag_type) {
        case Tag::Tip:
            return "[TIP]";
        case Tag::Request:
            return "[REQUEST]";
        case Tag::Error:
            return "[ERROR]";
        case Tag::Success:
            return "[SUCCESS]";
        case Tag::Info:
            return "[INFO]";
        case Tag::FatalError:
            return "[FATAL ERROR]";
        default:
            ThrowOnUnhandledCase();
    }
}

auto gw::Console::GetColorAsText(const Color color) const -> std::string_view {
    switch (color) {
        case Color::White:
            return "\x1b[37m";
        case Color::DarkGray:
            return "\x1b[90m";
        case Color::Green:
            return "\x1b[32m";
        case Color::Red:
            return "\x1b[31m";
        case Color::Magenta:
            return "\x1b[35m";
        case Color::Cyan:
            return "\x1b[36m";
        default:
            ThrowOnUnhandledCase();
    }
}
