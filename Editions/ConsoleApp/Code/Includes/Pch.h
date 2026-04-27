// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "WindowsHeader.h" // NOTE: Keep this as the first header, to get priority for defined macros inside

#ifdef _MSC_VER
    #pragma warning(push, 0)
    #pragma warning(disable: 4242) // sqlite_orm warning
    #pragma warning(disable: 4868) // sqlite_orm warning
    #pragma warning(disable: 4866) // sqlite_orm warning
    #pragma warning(disable: 5045) // sqlite_orm warning
    #pragma warning(disable: 6246) // sqlite_orm warning
    #include <sqlite_orm/sqlite_orm.h>
    #include <cpr/cpr.h>
    #pragma warning(pop)
#else
    #include <sqlite_orm/sqlite_orm.h>
    #include <cpr/cpr.h>
#endif

// IWYU pragma: begin_exports
#include <optional>
#include <string>
#include <format>
#include <print>
#include <iostream>
#include <cassert>
#include <exception>
#include <source_location>
#include <filesystem>
#include <vector>
#include <array>
#include <string_view>
#include <cstdio>
#include <memory>
#include <fstream>
#include <utility>
#include <chrono>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include <mutex>
#include <ranges>
// IWYU pragma: end_exports
