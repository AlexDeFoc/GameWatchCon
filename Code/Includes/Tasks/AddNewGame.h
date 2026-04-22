// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

namespace gw {
class Console;
class AppState;
class AppSettings;
class GameLibrary;
} // namespace gw

namespace gw::tasks {
[[nodiscard]] auto AddNewGame(gw::Console&, gw::AppState&, gw::AppSettings&, gw::GameLibrary&) -> void*;
} // namespace gw::tasks