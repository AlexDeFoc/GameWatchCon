#include "Pch.h"
#include "AppSettings.h"
#include "Tasks/ChangeAutoSaveInterval.h"
#include "Tasks/SettingsMenu.h"

namespace {
[[nodiscard]] auto GetFormattedInterval(std::chrono::seconds) noexcept -> std::string;
}

auto gw::tasks::ChangeAutoSaveInterval(gw::Console& console, gw::AppState&, gw::AppSettings& app_settings, gw::GameLibrary&) -> void* {
    const auto [new_interval, input_status] = console.RequestNewAutoSaveInterval(GetFormattedInterval(app_settings.GetAutoSaveInterval()));

    switch (input_status) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return (void*)gw::tasks::SettingsMenu;

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    app_settings.SetAutoSaveInterval(new_interval);

    return (void*)gw::tasks::SettingsMenu;
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