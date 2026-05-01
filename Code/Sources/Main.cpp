// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "TaskDispatcher.hpp"

int main() {
    try {
        gw::TaskDispatcher dispatcher{};
        dispatcher.Start();
    } catch (...) {
        return 0;
    }

    return 0;
}

