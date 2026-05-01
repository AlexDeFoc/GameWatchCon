// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "GameClockTimeTypes.hpp"
#include <string>
#include <glaze/glaze.hpp>

namespace gw {
class GameEntry {
private:
    std::string title;
    std::int64_t playtime{};

public:
    GameEntry() noexcept = default;
    GameEntry(std::string&& new_title) noexcept;

    [[nodiscard]] auto GetPlaytime() const noexcept -> gw::seconds;
    [[nodiscard]] auto GetTitle() const noexcept -> std::string_view;
    auto AddPlaytime(gw::seconds elapsed_time) noexcept -> void;
    auto SetGameTitle(std::string&& new_title) noexcept -> void;
    auto ResetPlaytime() noexcept -> void;

    struct glaze {
        using T = GameEntry;
        static constexpr auto value = glz::object(
            "title", &T::title,
            "playtime", &T::playtime);
    };
};
} // namespace gw
