#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "AppSettings.h"
#include "Tasks/ChangeAutoSaveInterval.h"
#include "Tasks/SettingsMenu.h"

namespace {
[[nodiscard]] auto GetFormattedInterval(std::chrono::seconds) noexcept -> std::string;
}

auto gw::tasks::ChangeAutoSaveInterval(gw::Console& console, gw::AppState&, gw::AppSettings& app_settings, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsAnyGameActive()) {
        console.WriteLineToCache(Console::Tag::Error, "Cannot change auto save interval while a game is active. Stop it first!");
        return Task{gw::tasks::SettingsMenu};
    }

    const auto [new_interval, input_status] = console.RequestNewAutoSaveInterval(GetFormattedInterval(app_settings.GetAutoSaveInterval()));

    switch (input_status) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return Task{gw::tasks::SettingsMenu};

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    app_settings.SetAutoSaveInterval(new_interval);

    return Task{gw::tasks::SettingsMenu};
}

namespace {
[[nodiscard]] auto GetFormattedInterval(std::chrono::seconds time_in_sec) noexcept -> std::string {
    using namespace std::chrono;

    const auto d = duration_cast<days>(time_in_sec);
    time_in_sec -= d;
    const auto h = duration_cast<hours>(time_in_sec);
    time_in_sec -= h;
    const auto m = duration_cast<minutes>(time_in_sec);
    time_in_sec -= m;

    struct ClockUnit {
        long long value;
        std::string_view label;
    };

    const std::array clock_units = {ClockUnit{d.count(), "day"},
                                    ClockUnit{h.count(), "h"},
                                    ClockUnit{m.count(), "min"},
                                    ClockUnit{time_in_sec.count(), "s"}};

    // clang-format off
    auto formatted_clock = clock_units | std::ranges::views::filter([](const ClockUnit& clk_unit) { return clk_unit.value > 0;})
                                       | std::ranges::views::transform([](const ClockUnit& clk_unit) { return std::format("{} {}{}", clk_unit.value, clk_unit.label, (clk_unit.label == "day" && clk_unit.value > 1 ? "s" : "")); })
                                       | std::ranges::views::join_with(std::string_view{" : "})
                                       | std::ranges::to<std::string>();
    // clang-format on

    return formatted_clock.empty() ? "0 s" : formatted_clock;
}
} // namespace