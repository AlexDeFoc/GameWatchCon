// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "GameEntryBase.h"
#include <vector>

namespace gw {
struct GamesLibraryFile {
    std::int64_t file_version{1};
    std::vector<GameEntryBase> games;
};
} // namespace gw
