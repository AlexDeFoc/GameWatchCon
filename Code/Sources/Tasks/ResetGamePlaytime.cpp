#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/ResetGamePlaytime.h"
#include "Tasks/EditGamesMenu.h"

namespace {
[[nodiscard]] auto GetFormattedPlaytime(std::chrono::milliseconds) noexcept -> std::string;
} // namespace

auto gw::tasks::ResetGamePlaytime(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> void* {
    auto list_games_func = [&] {
        const auto active_game_id = game_library.GetActiveGameId();
        for (std::size_t i{1}; const auto& game : game_library.GetAllGames()) {
            if (game_library.IsAnyGameActive() && i == active_game_id)
                Console::WriteLine("{}. {} - {} - {}", i, game.title, GetFormattedPlaytime(std::chrono::milliseconds(game.playtime_in_ms)), console.ColorText(gw::Console::Color::Red, "ACTIVE"));
            else
                Console::WriteLine("{}. {} - {}", i, game.title, GetFormattedPlaytime(std::chrono::milliseconds(game.playtime_in_ms)));
            
            ++i;
        }
    };

    const auto [selected_game_id, input_status_for_game_id] = console.RequestUserGameIDChoice(list_games_func, game_library.GetGameCount());

    switch (input_status_for_game_id) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return (void*)gw::tasks::EditGamesMenu;

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    const auto [user_is_sure, input_status_for_user_confirmation] = console.RequestUserConfirmation();

    switch (input_status_for_user_confirmation) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    if (user_is_sure) {
        game_library.ResetGamePlaytime(selected_game_id);
        console.WriteLineToCache(Console::Tag::Success, "Reset game playtime"); // TODO: Add more context
    } else
        console.WriteLineToCache(Console::Tag::Info, "Action cancelled");

    return (void*)gw::tasks::EditGamesMenu;
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