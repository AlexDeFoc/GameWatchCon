// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

// STD libraries
#include <cassert>
#include <charconv>
#include <chrono> // TODO: Remove/Replace (by manual optimization); Reason: too much overhead? maybe.
#include <cstddef>
#include <cstdint>
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// OS-specific libraries
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX
#include <Windows.h>
#endif

// External libraries
#include <cpr/cpr.h>