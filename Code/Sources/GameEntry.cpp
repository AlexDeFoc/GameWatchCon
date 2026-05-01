// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "GameEntry.hpp"

gw::GameEntry::GameEntry(std::string&& new_title) noexcept : title{std::move(new_title)} {}

auto gw::GameEntry::GetPlaytime() const noexcept -> seconds {
    return gw::seconds(playtime);
}

auto gw::GameEntry::GetTitle() const noexcept -> std::string_view {
    return title;
}

auto gw::GameEntry::AddPlaytime(const gw::seconds elapsed_time) noexcept -> void {
    playtime += elapsed_time.count();
}

auto gw::GameEntry::SetGameTitle(std::string&& new_title) noexcept -> void {
    title = std::move(new_title);
}

auto gw::GameEntry::ResetPlaytime() noexcept -> void {
    playtime = 0;
}

