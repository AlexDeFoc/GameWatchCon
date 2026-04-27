#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/CreateGamesDatabaseBackup.h"
#include "Tasks/SettingsMenu.h"

auto gw::tasks::CreateGamesDatabaseBackup(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> Task {
    game_library.CreateGamesDatabaseBackup();
    console.WriteLineToCache(Console::Tag::Success, "Created games database backup");
    return Task{gw::tasks::SettingsMenu};
}