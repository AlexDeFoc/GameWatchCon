// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.hxx"

auto gw::GameLibraryWriteAccess::SetGameTitle(const std::size_t index, std::string new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].SetTitle(std::move(new_title));
}

auto gw::GameLibraryWriteAccess::SetGameTitle(const std::size_t index, const std::string_view new_title) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].SetTitle(std::string{new_title});
}

auto gw::GameLibraryWriteAccess::ResetGameClock(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_[index].ResetClock();
}

auto gw::GameLibraryWriteAccess::RemoveGame(const std::size_t index) noexcept -> void {
    assert(index < games_.size() && "Provided out of range index to GameLibraryWriteAccess::SetGameTitle");
    games_.erase(games_.begin() + index);
}

auto gw::GameLibraryWriteAccess::AddGame(const std::string_view title) noexcept -> void { games_.emplace_back(std::string{title}); }

auto gw::GameLibraryWriteAccess::AddGame(std::string title) noexcept -> void { games_.emplace_back(std::move(title)); }