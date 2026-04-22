#include "Pch.h"
#include "Tasks/ListGames.h"
#include "Tasks/MainMenu.h"
#include "Console.h"
#include "GameLibrary.h"

namespace {
[[nodiscard]] auto GetFormattedPlaytime(std::chrono::milliseconds) noexcept -> std::string;
}

auto gw::tasks::ListGames(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> void* {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::Tag::Info, "No entries found!");
        return (void*)gw::tasks::MainMenu;
    }

    console.ClearCout();

    const auto active_game_id = game_library.GetActiveGameId();
    for (std::size_t i{1}; const auto& game : game_library.GetAllGames()) {
        if (game_library.IsAnyGameActive() && i == active_game_id)
            Console::WriteLine("{}. {} - {} - {}", i, game.title, GetFormattedPlaytime(std::chrono::milliseconds(game.playtime_in_ms)), console.ColorText(gw::Console::Color::Red, "ACTIVE"));
        else
            Console::WriteLine("{}. {} - {}", i, game.title, GetFormattedPlaytime(std::chrono::milliseconds(game.playtime_in_ms)));
    
        ++i;
    }

    console.WriteLine(Console::Tag::Tip, "Press any key to go back");
    console.RequestKeyPress();

    return (void*)gw::tasks::MainMenu;
}

namespace {
[[nodiscard]] auto GetFormattedPlaytime(std::chrono::milliseconds time_in_ms) noexcept -> std::string {
    using namespace std::chrono;

    auto ms = time_in_ms;
    const auto d = duration_cast<days>(ms);
    ms -= d;
    const auto h = duration_cast<hours>(ms);
    ms -= h;
    const auto m = duration_cast<minutes>(ms);
    ms -= m;
    const auto s = duration_cast<seconds>(ms);
    ms -= s;

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
    if (ms.count() > 0)
        bits.emplace_back(std::format("{} ms", ms.count()));

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