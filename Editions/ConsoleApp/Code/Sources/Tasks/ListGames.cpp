#include "Pch.h"
#include "Tasks/ListGames.h"
#include "Tasks/MainMenu.h"
#include "Console.h"
#include "GameLibrary.h"

auto gw::tasks::ListGames(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    if (game_library.IsEmpty()) {
        console.WriteLineToCache(Console::Tag::Info, "No entries found!");
        return Task{gw::tasks::MainMenu};
    }

    console.ClearCout();

    std::println("{}", game_library.GetPrintableGames(console));

    console.WriteLine(Console::Tag::Tip, "Press any key to go back");
    console.RequestKeyPress();

    return Task{gw::tasks::MainMenu};
}