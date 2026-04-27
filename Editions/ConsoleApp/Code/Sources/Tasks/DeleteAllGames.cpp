#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/DeleteAllGames.h"
#include "Tasks/SettingsMenu.h"

auto gw::tasks::DeleteAllGames(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsAnyGameActive()) {
        console.WriteLineToCache(Console::Tag::Error, "Cannot delete games while one is active. Stop it first!");
        return Task{gw::tasks::SettingsMenu};
    }

    const auto [user_is_sure, input_status_for_user_confirmation] = console.RequestUserConfirmation();

    switch (input_status_for_user_confirmation) {
        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    if (user_is_sure) {
        game_library.DeleteAllGames();
        console.WriteLineToCache(Console::Tag::Success, "Deleted all games");
    } else
        console.WriteLineToCache(Console::Tag::Info, "Action cancelled");

    return Task{gw::tasks::SettingsMenu};
}