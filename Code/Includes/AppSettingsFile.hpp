// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file
//
#pragma once

#include "AppSettingsDefaults.hpp"

namespace gw {
struct SettingsFile {
    std::int64_t file_version{1};
    bool auto_save_enabled_status{AppSettingsDefaults::auto_save_status_default};
    std::int64_t auto_save_interval_in_minutes{AppSettingsDefaults::auto_save_interval_default.count()};
};
} // namespace gw
