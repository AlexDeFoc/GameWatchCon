#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/AddnewGame.h"
#include "Tasks/MainMenu.h"

auto gw::tasks::AddNewGame(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> void* {
    auto [new_game_title, input_status] = console.RequestGameTitle();

    switch (input_status) {
        case Console::InputStatus::Cancelled:
            console.WriteLineToCache(Console::Tag::Info, "Action cancelled");
            return (void*)gw::tasks::MainMenu;

        case Console::InputStatus::Success:
            break;

        default:
            console.ThrowOnUnhandledCase();
    }

    game_library.AddNewGame(std::move(new_game_title));

    console.WriteLineToCache(Console::Tag::Success, "Added new game");

    return (void*)gw::tasks::MainMenu;
}