cmake_minimum_required(VERSION 4.3)
project(GameWatch LANGUAGES CXX)

# GameWatch - Console Edition
add_executable(GameWatchCon)

target_precompile_headers(GameWatchCon PRIVATE "Includes/Pch.h")

target_sources(GameWatchCon PRIVATE
    "Sources/Main.cpp"
    "Sources/Console.cpp"
    "Sources/DiskManager.cpp"
    "Sources/AppSettings.cpp"
    "Sources/TaskDispatcher.cpp"
    "Sources/AppState.cpp"
    "Sources/GameLibrary.cpp"
    "Sources/Tasks/StartApp.cpp"
    "Sources/Tasks/StopApp.cpp"
    "Sources/Tasks/MainMenu.cpp"
    "Sources/Tasks/ListGames.cpp"
    "Sources/Tasks/AddNewGame.cpp"
    "Sources/Tasks/SettingsMenu.cpp"
    "Sources/Tasks/ToggleAutoSaveStatus.cpp"
    "Sources/Tasks/ChangeAutoSaveInterval.cpp"
    "Sources/Tasks/CheckForUpdates.cpp"
    "Sources/Tasks/EditGamesMenu.cpp"
    "Sources/Tasks/ChangeGameTitle.cpp"
    "Sources/Tasks/ResetGamePlaytime.cpp"
    "Sources/Tasks/DeleteGame.cpp"
)

target_include_directories(GameWatchCon PRIVATE Includes)

set_target_properties(GameWatchCon PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Warnings only for the common generators you mentioned.
#if(MSVC OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    #target_compile_options(GameWatchCon PRIVATE
        #            /W4
        #/WX
        #/permissive-
        #)

    #else()
    #target_compile_options(GameWatchCon PRIVATE
        #            -Wall
        #            -Wextra
        #            -Wpedantic
        #            -Wshadow
        #            -Wnon-virtual-dtor
        #            -Wold-style-cast
        #            -Wunused
        #            -Woverloaded-virtual
        #            -Wconversion
        #            -Werror
        # )
    #endif()

find_package(cpr REQUIRED)
find_package(SqliteOrm REQUIRED)
target_link_libraries(GameWatchCon PRIVATE cpr::cpr sqlite_orm::sqlite_orm)
