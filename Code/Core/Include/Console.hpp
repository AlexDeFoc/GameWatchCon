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

#pragma once

#include "IListGames.hpp"
#include "IListOptions.hpp"

#include <expected>
#include <utility>
#include <print>
#include <string>
#include <format>
#include <stdexcept>
#include <iostream>

namespace gw::con {
class Console {
public:
    enum class MessageType { Tip,
                             Request,
                             Info,
                             Error };
    enum class InputStatus { Cancelled };

    [[nodiscard]] auto get_game_id(const IListGames &, const std::pair<int, int>) const noexcept -> std::expected<int, InputStatus>;

    [[nodiscard]] auto get_menu_choice(const IListOptions &, const std::pair<int, int>) const noexcept -> std::expected<int, InputStatus>;

    // TODO: Maybe add some more context when requesting title. Or add some rules, e.g: empty titles.
    [[nodiscard]] auto get_game_title() const noexcept -> std::expected<std::string, InputStatus>;

    [[nodiscard]] auto get_user_confirmation() const noexcept -> bool;

    template <typename... Args>
    constexpr auto print(const MessageType msg_type, const std::format_string<Args...> msg_format, Args &&...args) const noexcept -> void {
        std::print("{}{}:{} {}", get_msg_tag_color(msg_type), get_msg_tag(msg_type), _ansi_reset_color, std::format(msg_format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    constexpr auto println(const MessageType msg_type, const std::format_string<Args...> msg_format, Args &&...args) const noexcept -> void {
        std::println("{}{}:{} {}", get_msg_tag_color(msg_type), get_msg_tag(msg_type), _ansi_reset_color, std::format(msg_format, std::forward<Args>(args)...));
    }

private:
    static constexpr std::string_view _ansi_reset_color = {"\u001b[0m"};

    static constexpr auto get_msg_tag(const MessageType msg_type) noexcept -> std::string_view;

    static constexpr auto get_msg_tag_color(const MessageType msg_type) noexcept -> std::string_view;

    static constexpr auto clear_screen() -> void;
};
} // namespace gw::con