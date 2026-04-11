// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/Utils/Utils.hxx"

auto gw::utils::ColorText(const Console& console, const TextColor color, std::string uncolored_text) noexcept -> std::string {
    if (console.IsCapableDisplayingColoredText() == false)
        return uncolored_text;

    switch (color) {
        case TextColor::DarkGray:
            return std::string{std::format("{}{}{}", "\x1b[90m", std::move(uncolored_text), "\x1b[0m")};
        case TextColor::Green:
            return std::string{std::format("{}{}{}", "\x1b[32m", std::move(uncolored_text), "\x1b[0m")};
        case TextColor::Magenta:
            return std::string{std::format("{}{}{}", "\x1b[35m", std::move(uncolored_text), "\x1b[0m")};
        case TextColor::Red:
            return std::string{std::format("{}{}{}", "\x1b[31m", std::move(uncolored_text), "\x1b[0m")};

        default:
            assert(false && "Unhandled text color");
            std::terminate();
    }
}