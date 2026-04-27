#include "Pch.h"
#include "Tasks/ToggleGameClock.h"
#include "Tasks/MainMenu.h"
#include "Console.h"
#include "GameLibrary.h"

auto gw::tasks::ToggleGameClock(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::Tag::Info, "No entries found to start!");
        return Task{gw::tasks::MainMenu};
    }

    bool is_any_game_active = game_library.IsAnyGameActive();

    int selected_game_id{};
    if (!is_any_game_active) {
        const auto [gotten_selected_game_id, input_status_for_game_id] = console.RequestUserGameIDChoice(game_library.GetPrintableGames(console), game_library.GetGameCount());

        switch (input_status_for_game_id) {
            case Console::InputStatus::Cancelled:
                console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
                return Task{gw::tasks::MainMenu};

            case Console::InputStatus::Success:
                break;

            default:
                console.ThrowOnUnhandledCase();
        }

        selected_game_id = gotten_selected_game_id;
    }
    else {
        selected_game_id = game_library.GetActiveGameId();
    }

    game_library.ToggleGameClock(selected_game_id);

    return Task{gw::tasks::MainMenu};
}