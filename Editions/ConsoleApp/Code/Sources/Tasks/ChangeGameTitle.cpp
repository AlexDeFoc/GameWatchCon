#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/ChangeGameTitle.h"
#include "Tasks/EditGamesMenu.h"

auto gw::tasks::ChangeGameTitle(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    const auto [selected_game_id, input_status_for_game_id] = console.RequestUserGameIDChoice(game_library.GetPrintableGames(console), game_library.GetGameCount());

    switch (input_status_for_game_id) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return Task{gw::tasks::EditGamesMenu};

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    auto [new_game_title, input_status_for_game_title] = console.RequestGameTitle();

    switch (input_status_for_game_title) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return Task{gw::tasks::EditGamesMenu};

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    game_library.SetGameTitle(selected_game_id, std::move(new_game_title));

    // TODO: Add more context
    console.WriteLineToCache(Console::Tag::Success, "Changed game title");

    return Task{gw::tasks::EditGamesMenu};
}