cmake_minimum_required(VERSION 4.3)
project(GameWatchCon LANGUAGES CXX)

# Main App
add_executable(GameWatchCon)

target_precompile_headers(GameWatchCon PRIVATE "Code/pch.hxx")

target_include_directories(GameWatchCon PRIVATE Code)

target_sources(GameWatchCon PRIVATE
        # Core
        Code/Core/App/App.cxx
        Code/Core/AppConfig/AppConfig.cxx
        Code/Core/AppEntryPoint/Main.cxx
        Code/Core/AppState/AppState.cxx
        Code/Core/Console/Console.cxx
        Code/Core/GameClock/GameClock.cxx
        Code/Core/GameEntry/GameEntry.cxx
        Code/Core/GameLibrary/GameLibrary.cxx
        Code/Core/Utils/Utils.cxx

        # Tasks
        Code/Tasks/AddNewGame/AddNewGame.cxx
        Code/Tasks/ChangeAutoSaveInterval/ChangeAutoSaveInterval.cxx
        Code/Tasks/ChangeGameTitle/ChangeGameTitle.cxx
        Code/Tasks/CheckForUpdates/CheckForUpdates.cxx
        Code/Tasks/DeleteGame/DeleteGame.cxx
        Code/Tasks/EditGamesMenu/EditGamesMenu.cxx
        Code/Tasks/ListGames/ListGames.cxx
        Code/Tasks/MainMenu/MainMenu.cxx
        Code/Tasks/ResetGameClock/ResetGameClock.cxx
        Code/Tasks/SettingsMenu/SettingsMenu.cxx
        Code/Tasks/StartApp/StartApp.cxx
        Code/Tasks/StopApp/StopApp.cxx
        Code/Tasks/ToggleAutoSave/ToggleAutoSave.cxx
        Code/Tasks/ToggleGameClock/ToggleGameClock.cxx
)

target_include_directories(GameWatchCon PRIVATE Code)

set_target_properties(GameWatchCon PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Warnings only for the common generators you mentioned.
if(CMAKE_GENERATOR MATCHES "Ninja|Unix Makefiles|Visual Studio")
    if(MSVC)
        # Visual Studio / cl.exe / clang-cl
        target_compile_options(GameWatchCon PRIVATE
            /W4
            /permissive-
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # GCC / Clang
        target_compile_options(GameWatchCon PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()
endif()

find_package(cpr CONFIG REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(GameWatchCon PRIVATE cpr::cpr sqlite_orm::sqlite_orm)