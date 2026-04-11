// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.hxx"
#include "Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.hxx"

namespace gw {
class GameLibrary : public GameLibraryReadAccess,
                    public GameLibraryWriteAccess {
};
} // namespace gw