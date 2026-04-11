// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/GameLibrary/Partitions/Base.hxx"

namespace gw {
class GameLibraryReadAccess : virtual public GameLibraryBase {
public:
    auto ListGames() const noexcept -> void;

    [[nodiscard]] auto IsEmpty() const noexcept -> bool;

    [[nodiscard]] auto GetGameTitle(std::size_t) const noexcept -> std::string_view;

    [[nodiscard]] auto GamesCount() const noexcept -> std::size_t;
};
} // namespace gw