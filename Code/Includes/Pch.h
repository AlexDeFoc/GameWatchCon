// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

// IWYU pragma: begin_exports
#include "PlatformSpecificHeaders.h" // NOTE: Keep this as the first header, to get priority for defined macros inside
#include <glaze/glaze.hpp>
#include <cpr/cpr.h>
#include <climits> // PATH_MAX
#include <cstring>
#include <cstdlib>
#include <span>
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
