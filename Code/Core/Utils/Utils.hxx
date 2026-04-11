// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/Console/Console.hxx"

namespace gw::utils {
enum class TextColor : int {
    DarkGray,
    Green,
    Magenta,
    Red
};

[[nodiscard]] auto ColorText(const Console&, TextColor, std::string) noexcept -> std::string;
} // namespace gw::utils