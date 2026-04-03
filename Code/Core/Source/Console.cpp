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

#include "Console.hpp"

auto gw::con::Console::get_game_id(const IListGames &library, const std::pair<int, int> bounds) const noexcept -> std::expected<int, InputStatus> {
    while (true) {
        library.list_games();
        println(MessageType::Tip, "Enter CTRL+Z to cancel");
        print(MessageType::Request, "Enter game id: ");

        if (std::string input; std::getline(std::cin, input)) {
            try {
                if (const auto id = std::stoi(input); id >= bounds.first && id <= bounds.second) { return id; }

                clear_screen();
                println(MessageType::Error, "Invalid input!");
            } catch (std::invalid_argument &) {
                clear_screen();
                println(MessageType::Error, "Invalid input!");
            } catch (std::out_of_range &) {
                clear_screen();
                println(MessageType::Error, "Input out of range!");
            }
        } else if (std::cin.eof()) {
            std::cin.clear();
            return std::unexpected(InputStatus::Cancelled);
        } else if (std::cin.fail()) {
            std::cin.clear();
            clear_screen();
            println(MessageType::Error, "An unexpected error occurred!");
        }
    }
}

auto gw::con::Console::get_menu_choice(const IListOptions &menu, const std::pair<int, int> bounds) const noexcept -> std::expected<int, InputStatus> {
    while (true) {
        menu.list_options();
        println(MessageType::Tip, "Enter CTRL+Z to cancel");
        print(MessageType::Request, "Enter option id: ");

        if (std::string input; std::getline(std::cin, input)) {
            try {
                if (const auto id = std::stoi(input); id >= bounds.first && id <= bounds.second) { return id; }

                clear_screen();
                println(MessageType::Error, "Invalid input!");
            } catch (std::invalid_argument &) {
                clear_screen();
                println(MessageType::Error, "Invalid input!");
            } catch (std::out_of_range &) {
                clear_screen();
                println(MessageType::Error, "Input out of range!");
            }
        } else if (std::cin.eof()) {
            std::cin.clear();
            return std::unexpected(InputStatus::Cancelled);
        } else if (std::cin.fail()) {
            std::cin.clear();
            clear_screen();
            println(MessageType::Error, "An unexpected error occurred!");
        }
    }
}

auto gw::con::Console::get_game_title() const noexcept -> std::expected<std::string, InputStatus> {
    while (true) {
        println(MessageType::Tip, "Enter CTRL+Z to cancel");
        print(MessageType::Request, "Enter new game title: ");

        if (std::string input; std::getline(std::cin, input)) { return input; }

        if (std::cin.eof()) {
            std::cin.clear();
            return std::unexpected(InputStatus::Cancelled);
        }

        if (std::cin.fail()) {
            std::cin.clear();
            clear_screen();
            println(MessageType::Error, "An unexpected error occurred!");
        }
    }
}

auto gw::con::Console::get_user_confirmation() const noexcept -> bool {
    while (true) {
        std::println("Are you sure?\n1.Yes\n0.No");
        print(MessageType::Request, "Enter option id: ");

        if (std::string input; std::getline(std::cin, input)) {
            clear_screen();

            try {
                if (const auto id = std::stoi(input); id == 1 || id == 0) return id;

                println(MessageType::Error, "Invalid input!");
            } catch (std::invalid_argument &) {
                println(MessageType::Error, "Invalid input!");
            } catch (std::out_of_range &) { println(MessageType::Error, "Input out of range!"); }
        } else if (std::cin.eof()) {
            std::cin.clear();
        } else if (std::cin.fail()) {
            std::cin.clear();
            clear_screen();
            println(MessageType::Error, "An unexpected error occurred!");
        }
    }
}

constexpr auto gw::con::Console::get_msg_tag(const MessageType msg_type) noexcept -> std::string_view {
    switch (msg_type) {
        case MessageType::Info:
            return "[Info]";

        case MessageType::Tip:
            return "[Tip]";

        case MessageType::Request:
            return "[Request]";

        case MessageType::Error:
            return "[Error]";

        default:
            throw std::logic_error({});
    }
}

constexpr auto gw::con::Console::get_msg_tag_color(const MessageType msg_type) noexcept -> std::string_view {
    switch (msg_type) {
        case MessageType::Info:
            return "\u001b[37m";

        case MessageType::Tip:
            return "\u001b[32m";

        case MessageType::Request:
            return "\u001b[35m";

        case MessageType::Error:
            return "\u001b[31m";

        default:
            throw std::logic_error({});
    }
}

constexpr void gw::con::Console::clear_screen() {
    std::println("\x1b[2J\x1b[3J\x1b[H");
}