// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/TaskType/TaskType.hxx"

namespace gw::tasks {
[[nodiscard]] auto StartApp(bool&) noexcept -> TaskType;
}