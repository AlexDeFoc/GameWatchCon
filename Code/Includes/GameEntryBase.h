// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <string>

namespace gw {
struct GameEntryBase {
public:
    std::string title;
    std::int64_t playtime;
};
} // namespace gw
