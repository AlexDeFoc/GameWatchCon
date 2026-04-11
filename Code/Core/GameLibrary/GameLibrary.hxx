// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/GameEntry/GameEntry.hxx"

namespace gw {
class GameLibrary {
public:
    auto SetGameTitle(std::size_t, std::string) noexcept -> void;

    auto SetGameTitle(std::size_t, std::string_view) noexcept -> void;

    auto ResetGameClock(std::size_t) noexcept -> void;

    auto RemoveGame(std::size_t) noexcept -> void;

    auto AddGame(std::string_view) noexcept -> void;

    auto AddGame(std::string) noexcept -> void;

    auto ListGames() const noexcept -> void;

    [[nodiscard]] auto IsEmpty() const noexcept -> bool;

    [[nodiscard]] auto GetGameTitle(std::size_t) const noexcept -> std::string_view;

    [[nodiscard]] auto GamesCount() const noexcept -> std::size_t;

private:
    std::vector<GameEntry> games_;
};
} // namespace gw