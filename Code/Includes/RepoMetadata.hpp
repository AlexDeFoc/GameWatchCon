// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file


#pragma once

#include <string>
#include <vector>

namespace gw {
struct AppVersionFile {
    std::string version{"1.1.0"};
};

struct AppUpdateChangeInFile {
    std::string version{"1.1.0"};
    std::string type{"General"};
    std::string description{"Made the app a bit better"};
};

struct AppChangesLogFile {
    std::int64_t file_version{1};
    std::vector<AppUpdateChangeInFile> changes;
};
} // namespace gw
