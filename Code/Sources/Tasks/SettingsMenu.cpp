#include "Pch.h"
#include "Console.h"
#include "AppSettings.h"
#include "Tasks/SettingsMenu.h"
#include "Tasks/MainMenu.h"
#include "Tasks/ToggleAutoSaveStatus.h"
#include "Tasks/ChangeAutoSaveInterval.h"

namespace {
[[nodiscard]] auto GetFormattedInterval(std::chrono::seconds) noexcept -> std::string;
}

auto gw::tasks::SettingsMenu(gw::Console& console, gw::AppState&, gw::AppSettings& app_settings, gw::GameLibrary&) -> void* {
    auto menu_opts = [&] -> const std::array<std::string, 3> {
        return {std::format("Toggle game auto save: {}", app_settings.IsAutoSaveEnabled() == true ? console.ColorText(Console::Color::Green, "enabled") : console.ColorText(Console::Color::Red, "disabled")),
                std::format("Change game auto save interval: {}", console.ColorText(gw::Console::Color::Green, GetFormattedInterval(app_settings.GetAutoSaveInterval()))),
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
            return (void*)gw::tasks::MainMenu;

        case 1:
            return (void*)gw::tasks::ToggleAutoSaveStatus;

        case 2:
            return (void*)gw::tasks::ChangeAutoSaveInterval;

        default:
            console.ThrowOnUnhandledCase();
            return nullptr;
    }
}

namespace {
[[nodiscard]] auto GetFormattedInterval(std::chrono::seconds time_in_sec) noexcept -> std::string {
    using namespace std::chrono;

    auto s = time_in_sec;
    const auto d = duration_cast<days>(s);
    s -= d;
    const auto h = duration_cast<hours>(s);
    s -= h;
    const auto m = duration_cast<minutes>(s);
    s -= m;

    std::vector<std::string> bits{};
    if (d.count() > 0) {
        if (d.count() > 1)
            bits.emplace_back(std::format("{} days", d.count()));
        else
            bits.emplace_back(std::format("{} day", d.count()));
    }

    if (h.count() > 0)
        bits.emplace_back(std::format("{} h", h.count()));
    if (m.count() > 0)
        bits.emplace_back(std::format("{} min", m.count()));
    if (s.count() > 0)
        bits.emplace_back(std::format("{} s", s.count()));

    if (bits.empty())
        return "0 s";

    std::string printable_duration{};
    for (std::size_t i{0}; i < bits.size(); ++i) {
        printable_duration += bits[i];

        if (i < bits.size() - 1)
            printable_duration += " : ";
    }

    return printable_duration;
}
} // namespace