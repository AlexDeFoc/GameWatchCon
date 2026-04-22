#include "Pch.h"
#include "Console.h"
#include "GameLibrary.h"
#include "Tasks/MainMenu.h"
#include "Tasks/StopApp.h"
#include "Tasks/ListGames.h"
#include "Tasks/EditGamesMenu.h"
#include "Tasks/AddNewGame.h"
#include "Tasks/SettingsMenu.h"
#include "Tasks/CheckForUpdates.h"

auto gw::tasks::MainMenu(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary& game_library) -> void* {
    auto menu_opts = [&] -> const std::array<std::string, 7> {
        if (game_library.IsAnyGameActive()) {
            return {
                "List games",
                std::format("Stop game: {}", game_library.ActiveGameTitle()),
                "Edit games",
                "Add new game",
                "Settings",
                "Check for updates",
                "Exit App"};
        }

        return {"List games",
                "Start game",
                "Edit games",
                "Add new game",
                "Settings",
                "Check for updates",
                "Exit App"};
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
            return (void*)gw::tasks::StopApp;

        case 1:
            return (void*)gw::tasks::ListGames;

        case 3:
            return (void*)gw::tasks::EditGamesMenu;

        case 4:
            return (void*)gw::tasks::AddNewGame;

        case 5:
            return (void*)gw::tasks::SettingsMenu;

        case 6:
            return (void*)gw::tasks::CheckForUpdates;

        default:
            console.ThrowOnUnhandledCase();
            return nullptr;
    }
}