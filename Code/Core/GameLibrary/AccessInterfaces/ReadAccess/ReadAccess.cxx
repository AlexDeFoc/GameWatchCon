// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.hxx"

auto gw::GameLibraryReadAccess::ListGames() const noexcept -> void {
    for (std::size_t game_index{1}; const auto& game : games_) {
        std::println("{}. {} - {}", game_index, game.GetTitle(), game.GetPrintableClock());
        game_index++;
    }
}

auto gw::GameLibraryReadAccess::IsEmpty() const noexcept -> bool { return games_.empty(); }

auto gw::GameLibraryReadAccess::GetGameTitle(const std::size_t index) const noexcept -> std::string_view {
    assert(index < games_.size() && "Provided out of range index to GameLibraryReadAccess::GetGameTitle");
    return games_[index].GetTitle();
}

auto gw::GameLibraryReadAccess::GamesCount() const noexcept -> std::size_t { return games_.size(); }