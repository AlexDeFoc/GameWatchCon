// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <chrono>

namespace gw {
using seconds = std::chrono::duration<std::int64_t, std::chrono::seconds::period>;
using minutes = std::chrono::duration<std::int64_t, std::chrono::minutes::period>;
using hours = std::chrono::duration<std::int64_t, std::chrono::hours::period>;
using days = std::chrono::duration<std::int64_t, std::chrono::days::period>;
} // namespace gw
