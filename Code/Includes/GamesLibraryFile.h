// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "GameEntry.h"
#include <vector>

namespace gw {
struct GamesLibraryCpyFile {
    GamesLibraryCpyFile() noexcept = default;

    std::int64_t file_version{1};
    std::vector<GameEntry> games{};

    struct glaze {
        using T = GamesLibraryCpyFile;
        static constexpr auto value = glz::object(
            "file_version", &T::file_version,
            "games", &T::games);
    };
};

struct GamesLibraryRefFile {
    GamesLibraryRefFile() noexcept = default;
    GamesLibraryRefFile(std::vector<GameEntry>* games_ptr) noexcept : games{games_ptr} {}

    std::int64_t file_version{1};
    std::vector<GameEntry>* games{nullptr};

    struct glaze {
        using T = GamesLibraryRefFile;
        static constexpr auto value = glz::object(
            "file_version", &T::file_version,
            "games", &T::games);
    };
};
} // namespace gw
