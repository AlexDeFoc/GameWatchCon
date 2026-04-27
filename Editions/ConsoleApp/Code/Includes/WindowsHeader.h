// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#ifdef _WIN32
    #ifndef UNICODE
        #define UNICODE
    #endif

    #ifndef _UNICODE
        #define _UNICODE
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #ifndef NOGDI
        #define NOGDI
    #endif

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

    #ifndef INCLUDED_WINDOWS_HEADER
        #define INCLUDED_WINDOWS_HEADER
        #include <Windows.h>
    #endif
#endif