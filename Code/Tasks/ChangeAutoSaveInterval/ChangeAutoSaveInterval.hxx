// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/AppConfig/AppConfig.hxx"
#include "Core/Console/Console.hxx"
#include "Core/GameLibrary/GameLibrary.hxx"
#include "Core/TaskType/TaskType.hxx"

namespace gw::tasks {
[[nodiscard]] auto ChangeAutoSaveInterval(Console&, AppConfig&, const GameLibrary&) noexcept -> TaskType;
} // namespace gw::tasks