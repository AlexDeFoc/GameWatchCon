cmake_minimum_required(VERSION 4.2)
project(GameWatchCon LANGUAGES CXX)

# Main App
add_executable(GameWatchCon)

target_sources(GameWatchCon PUBLIC FILE_SET CXX_MODULES FILES
        # Core
        Code/App/Core/App/App.ixx
        Code/App/Core/AppConfig/AppConfig.ixx
        Code/App/Core/AppState/AppState.ixx
        Code/App/Core/AppState/Interfaces/AppStatusAccess/AppStatusAccess.ixx
        Code/App/Core/Console/Console.ixx
        Code/App/Core/GameClock/GameClock.ixx
        Code/App/Core/GameEntry/GameEntry.ixx
        Code/App/Core/GameLibrary/GameLibrary.ixx
        Code/App/Core/GameLibrary/Partitions/Base.ixx
        Code/App/Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.ixx
        Code/App/Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.ixx
        Code/App/Core/Task/Task.ixx
        Code/App/Core/TaskQueue/TaskQueue.ixx

        # Tasks
        Code/App/Tasks/AddNewGame/AddNewGame.ixx
        Code/App/Tasks/ChangeGameTitle/ChangeGameTitle.ixx
        Code/App/Tasks/DeleteGame/DeleteGame.ixx
        Code/App/Tasks/EditGamesMenu/EditGamesMenu.ixx
        Code/App/Tasks/ListGames/ListGames.ixx
        Code/App/Tasks/MainMenu/MainMenu.ixx
        Code/App/Tasks/ResetGameClock/ResetGameClock.ixx
        Code/App/Tasks/StartApp/StartApp.ixx
        Code/App/Tasks/StopApp/StopApp.ixx
)

target_sources(GameWatchCon PRIVATE
        # Core
        Code/App/Core/App/App.cxx
        Code/App/Core/AppEntryPoint/Main.cxx
        Code/App/Core/AppState/Interfaces/AppStatusAccess/AppStatusAccess.cxx
        Code/App/Core/Console/Console.cxx
        Code/App/Core/GameClock/GameClock.cxx
        Code/App/Core/GameEntry/GameEntry.cxx
        Code/App/Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.cxx
        Code/App/Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.cxx
        Code/App/Core/Task/Task.cxx
        Code/App/Core/TaskQueue/TaskQueue.cxx

        # Tasks
        Code/App/Tasks/AddNewGame/AddNewGame.cxx
        Code/App/Tasks/ChangeGameTitle/ChangeGameTitle.cxx
        Code/App/Tasks/DeleteGame/DeleteGame.cxx
        Code/App/Tasks/EditGamesMenu/EditGamesMenu.cxx
        Code/App/Tasks/ListGames/ListGames.cxx
        Code/App/Tasks/MainMenu/MainMenu.cxx
        Code/App/Tasks/ResetGameClock/ResetGameClock.cxx
        Code/App/Tasks/StartApp/StartApp.cxx
        Code/App/Tasks/StopApp/StopApp.cxx
)

target_include_directories(GameWatchCon PRIVATE Code/App/)

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