// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/GameLibrary/GameLibrary.hxx"

auto gw::GameLibrary::SetGameTitle(const std::size_t index, std::string new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].SetTitle(std::move(new_title));
}

auto gw::GameLibrary::SetGameTitle(const std::size_t index, const std::string_view new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].SetTitle(std::string{new_title});
}

auto gw::GameLibrary::ResetGameClock(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].ResetClock();
}

auto gw::GameLibrary::RemoveGame(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_.erase(games_.begin() + index);
}

auto gw::GameLibrary::AddGame(const std::string_view title) noexcept -> void { games_.emplace_back(std::string{title}); }

auto gw::GameLibrary::AddGame(std::string title) noexcept -> void { games_.emplace_back(std::move(title)); }

auto gw::GameLibrary::ListGames() const noexcept -> void {
    for (std::size_t game_index{1}; const auto& game : games_) {
        std::println("{}. {} - {}", game_index, game.GetTitle(), game.GetPrintableClock());
        game_index++;
    }
}

auto gw::GameLibrary::IsEmpty() const noexcept -> bool { return games_.empty(); }

auto gw::GameLibrary::GetGameTitle(const std::size_t index) const noexcept -> std::string_view {
    assert(index < games_.size() && "Provided out of range index to GameLibraryReadAccess::GetGameTitle");
    return games_[index].GetTitle();
}

auto gw::GameLibrary::GamesCount() const noexcept -> std::size_t { return games_.size(); }