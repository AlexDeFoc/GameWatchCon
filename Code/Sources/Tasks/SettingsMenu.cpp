// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Console.hpp"
#include "AppSettings.hpp"
#include "Tasks/SettingsMenu.hpp"
#include "Tasks/MainMenu.hpp"
#include "Tasks/ToggleAutoSaveStatus.hpp"
#include "Tasks/ChangeAutoSaveInterval.hpp"
#include "Tasks/RestoreSettingsDefaults.hpp"
#include "Tasks/CreateGamesDatabaseBackup.hpp"
#include "Tasks/ResetAllGamesPlaytime.hpp"
#include "Tasks/DeleteAllGames.hpp"

namespace {
[[nodiscard]] auto GetFormattedInterval(std::chrono::seconds) noexcept -> std::string;
}

auto gw::tasks::SettingsMenu(gw::Console& console, gw::AppState&, gw::AppSettings& app_settings, gw::GameLibrary&) -> Task {
    auto menu_opts = [&] -> std::array<std::string, 7> {
        return {std::format("Toggle game auto save: {}", app_settings.IsAutoSaveEnabled() == true ? console.ColorText(Console::Color::Green, "enabled") : console.ColorText(Console::Color::Red, "disabled")),
                std::format("Change game auto save interval: {}", console.ColorText(gw::Console::Color::Green, GetFormattedInterval(app_settings.GetAutoSaveInterval()))),
                "Restore Settings Default",
                "Create games database backup",
                "Reset all games playtime",
                "Delete all games",
                "Go back"};
    };

    const auto [opt_sel, input_status] = console.RequestUserMenuChoice(menu_opts(), false);

    switch (input_status) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    switch (opt_sel) {
        case 0:
            return Task{gw::tasks::MainMenu};

        case 1:
            return Task{gw::tasks::ToggleAutoSaveStatus};

        case 2:
            return Task{gw::tasks::ChangeAutoSaveInterval};

        case 3:
            return Task{gw::tasks::RestoreSettingsDefaults};

        case 4:
            return Task{gw::tasks::CreateGamesDatabaseBackup};

        case 5:
            return Task{gw::tasks::ResetAllGamesPlaytime};

        case 6:
            return Task{gw::tasks::DeleteAllGames};

        default:
            console.ThrowOnUnhandledCase();
    }
}

namespace {
[[nodiscard]] auto GetFormattedInterval(std::chrono::seconds time_in_sec) noexcept -> std::string {
    using namespace std::chrono;
    using namespace std::string_view_literals;

    // clang-format off
    const auto d = duration_cast<days>(time_in_sec); time_in_sec -= d;
    const auto h = duration_cast<hours>(time_in_sec); time_in_sec -= h;
    const auto m = duration_cast<minutes>(time_in_sec); time_in_sec -= m;

    auto values = std::array<long long, 4>{{d.count(), h.count(), m.count(), time_in_sec.count()}};
    auto labels = std::array<std::string_view, 4>{{"day"sv, "h"sv, "min"sv, "s"sv}};

    auto duration_str = std::views::zip(values, labels)
                      | std::views::filter([](auto&& tuple){ return std::get<0>(tuple) > 0; })
                      | std::views::transform([](auto&& tuple) {
                          auto [val, label] = tuple;
                          return std::format("{} {}{}", val, label, (label == "day" && val > 1 ? "s" : ""));
                      })
                      | std::views::join_with(" : "sv)
                      | std::ranges::to<std::string>();
    // clang-format on

    return duration_str.empty() ? "0 s" : duration_str;
}
} // namespace
