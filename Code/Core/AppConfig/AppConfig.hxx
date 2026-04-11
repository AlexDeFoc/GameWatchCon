// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#pragma once

#include "pch.hxx"

namespace gw {
class AppConfig {
public:
    enum class AutoSaveStatus : bool { Disabled,
                                       Enabled };
    AppConfig() noexcept;

    auto ToggleAutoSaveStatus() noexcept -> void;
    auto ChangeAutoSaveInterval(std::chrono::steady_clock::duration) noexcept -> void;
    [[nodiscard]] auto GetAutoSaveStatus() const noexcept -> AutoSaveStatus;
    [[nodiscard]] auto GetPrintableAutoSaveInterval() const noexcept -> std::string;

private:
    AutoSaveStatus autosave_enabled_status_;
    std::chrono::steady_clock::duration autosave_interval_;
};
} // namespace gw