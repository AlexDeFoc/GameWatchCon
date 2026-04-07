cmake_minimum_required(VERSION 4.2)
project(GameWatchCon LANGUAGES CXX)

# Main App
add_executable(GameWatchCon)

target_sources(GameWatchCon PUBLIC FILE_SET CXX_MODULES FILES
        Code/App/Core/GameLibrary/GameLibrary.ixx
        Code/App/Core/GameLibrary/Partitions/Base.ixx
        Code/App/Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.ixx
        Code/App/Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.ixx

        Code/App/Core/GameClock/GameClock.ixx
        Code/App/Core/GameEntry/GameEntry.ixx

        Code/App/Core/Console/Console.ixx
        Code/App/Core/Console/Interfaces/ConsoleComponents/ConsoleComponents.ixx
        Code/App/Core/Console/Interfaces/ConsoleWriteAccess/ConsoleWriteAccess.ixx
        Code/App/Core/Console/Interfaces/ConsoleReadAccess/ConsoleReadAccess.ixx

        Code/App/Core/Task/Task.ixx

        Code/App/Core/TaskQueue/TaskQueue.ixx

        Code/App/Core/AppConfig/AppConfig.ixx

        Code/App/Core/AppState/AppState.ixx
        Code/App/Core/AppState/Interfaces/AppStatusAccess/AppStatusAccess.ixx

        Code/App/Tasks/StartApp/StartApp.ixx
        Code/App/Tasks/StopApp/StopApp.ixx
        Code/App/Tasks/GetMainMenuOptionChoice/GetMainMenuOptionChoice.ixx
        Code/App/Tasks/ValidateMainMenuOptionChoice/ValidateMainMenuOptionChoice.ixx
        Code/App/Tasks/ForwardUserFromMainMenu/ForwardUserFromMainMenu.ixx
        Code/App/Tasks/ListGames/ListGames.ixx
        Code/App/Tasks/AddNewGame/AddNewGame.ixx
        Code/App/Tasks/GetEditGamesMenuOptionChoice/GetEditGamesMenuOptionChoice.ixx
        Code/App/Tasks/ValidateEditGamesMenuOptionChoice/ValidateEditGamesMenuOptionChoice.ixx
        Code/App/Tasks/ForwardUserFromEditGamesMenu/ForwardUserFromEditGamesMenu.ixx
        Code/App/Tasks/GetGameIDUsedToChangeGameTitle/GetGameIDUsedToChangeGameTitle.ixx
        Code/App/Tasks/ValidateGameIDUsedToChangeGameTitle/ValidateGameIDUsedToChangeGameTitle.ixx
        Code/App/Tasks/GetGameTitleUsedToChangeGameTitle/GetGameTitleUsedToChangeGameTitle.ixx
        Code/App/Tasks/ValidateGameTitleUsedToChangeGameTitle/ValidateGameTitleUsedToChangeGameTitle.ixx
        Code/App/Tasks/ChangeGameTitle/ChangeGameTitle.ixx
        Code/App/Tasks/GetGameIDUsedToResetGameClock/GetGameIDUsedToResetGameClock.ixx
        Code/App/Tasks/ValidateGameIDUsedToResetGameClock/ValidateGameIDUsedToResetGameClock.ixx
        Code/App/Tasks/GetUserConfirmationUsedToResetGameClock/GetUserConfirmationUsedToResetGameClock.ixx
        Code/App/Tasks/ValidateUserConfirmationUsedToResetGameClock/ValidateUserConfirmationUsedToResetGameClock.ixx
        Code/App/Tasks/ResetGameClock/ResetGameClock.ixx

        Code/App/Core/App/App.ixx
)

target_sources(GameWatchCon PRIVATE
        Code/App/Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.cxx
        Code/App/Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.cxx

        Code/App/Core/GameClock/GameClock.cxx
        Code/App/Core/GameEntry/GameEntry.cxx

        Code/App/Core/Console/Interfaces/ConsoleWriteAccess/ConsoleWriteAccess.cxx
        Code/App/Core/Console/Interfaces/ConsoleReadAccess/ConsoleReadAccess.cxx

        Code/App/Core/Task/Task.cxx

        Code/App/Core/TaskQueue/TaskQueue.cxx

        Code/App/Core/AppState/Interfaces/AppStatusAccess/AppStatusAccess.cxx

        Code/App/Tasks/StartApp/StartApp.cxx
        Code/App/Tasks/StopApp/StopApp.cxx
        Code/App/Tasks/GetMainMenuOptionChoice/GetMainMenuOptionChoice.cxx
        Code/App/Tasks/ValidateMainMenuOptionChoice/ValidateMainMenuOptionChoice.cxx
        Code/App/Tasks/ForwardUserFromMainMenu/ForwardUserFromMainMenu.cxx
        Code/App/Tasks/ListGames/ListGames.cxx
        Code/App/Tasks/AddNewGame/AddNewGame.cxx
        Code/App/Tasks/GetEditGamesMenuOptionChoice/GetEditGamesMenuOptionChoice.cxx
        Code/App/Tasks/ValidateEditGamesMenuOptionChoice/ValidateEditGamesMenuOptionChoice.cxx
        Code/App/Tasks/ForwardUserFromEditGamesMenu/ForwardUserFromEditGamesMenu.cxx
        Code/App/Tasks/GetGameIDUsedToChangeGameTitle/GetGameIDUsedToChangeGameTitle.cxx
        Code/App/Tasks/ValidateGameIDUsedToChangeGameTitle/ValidateGameIDUsedToChangeGameTitle.cxx
        Code/App/Tasks/GetGameTitleUsedToChangeGameTitle/GetGameTitleUsedToChangeGameTitle.cxx
        Code/App/Tasks/ValidateGameTitleUsedToChangeGameTitle/ValidateGameTitleUsedToChangeGameTitle.cxx
        Code/App/Tasks/ChangeGameTitle/ChangeGameTitle.cxx
        Code/App/Tasks/GetGameIDUsedToResetGameClock/GetGameIDUsedToResetGameClock.cxx
        Code/App/Tasks/ValidateGameIDUsedToResetGameClock/ValidateGameIDUsedToResetGameClock.cxx
        Code/App/Tasks/GetUserConfirmationUsedToResetGameClock/GetUserConfirmationUsedToResetGameClock.cxx
        Code/App/Tasks/ValidateUserConfirmationUsedToResetGameClock/ValidateUserConfirmationUsedToResetGameClock.cxx
        Code/App/Tasks/ResetGameClock/ResetGameClock.cxx

        Code/App/Core/App/App.cxx

        Code/App/Core/AppEntryPoint/Main.cxx
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