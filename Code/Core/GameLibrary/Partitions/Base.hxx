// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/GameEntry/GameEntry.hxx"

namespace gw {
class GameLibraryBase {
public:
    GameLibraryBase() noexcept = default;
    virtual ~GameLibraryBase() = default;

protected:
    std::vector<GameEntry> games_;
};
} // namespace gw