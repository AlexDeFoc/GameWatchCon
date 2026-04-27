// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include <chrono>

using namespace std::chrono_literals;

namespace gw {
struct AppSettingsDefaults {
    static constexpr int auto_save_status_default{1};
    static constexpr auto auto_save_interval_default{5min};
};
} // namespace gw