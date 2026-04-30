// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

namespace gw {
class Console;
class AppState;
class AppSettings;
class GameLibrary;

struct Task {
    using Func = auto (*)(Console&, AppState&, AppSettings&, GameLibrary&) -> Task;
    Func func = nullptr;
};
}