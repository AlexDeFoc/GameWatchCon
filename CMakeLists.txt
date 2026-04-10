cmake_minimum_required(VERSION 4.3)
project(GameWatchCon LANGUAGES CXX)

# Main App
add_executable(GameWatchCon)

target_sources(GameWatchCon PUBLIC FILE_SET CXX_MODULES FILES
        # Core
        Code/Core/App/App.ixx
        Code/Core/TaskType/TaskType.ixx
        Code/Core/AppConfig/AppConfig.ixx
        Code/Core/Console/Console.ixx
        Code/Core/GameClock/GameClock.ixx
        Code/Core/GameEntry/GameEntry.ixx
        Code/Core/GameLibrary/Partitions/Base.ixx
        Code/Core/GameLibrary/GameLibrary.ixx
        Code/Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.ixx
        Code/Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.ixx
        Code/Core/Utils/Utils.ixx

        # Tasks
        Code/Tasks/Tasks.ixx
        Code/Tasks/AddNewGame/AddNewGame.ixx
        Code/Tasks/ChangeAutoSaveInterval/ChangeAutoSaveInterval.ixx
        Code/Tasks/ChangeGameTitle/ChangeGameTitle.ixx
        Code/Tasks/DeleteGame/DeleteGame.ixx
        Code/Tasks/EditGamesMenu/EditGamesMenu.ixx
        Code/Tasks/ListGames/ListGames.ixx
        Code/Tasks/MainMenu/MainMenu.ixx
        Code/Tasks/ResetGameClock/ResetGameClock.ixx
        Code/Tasks/SettingsMenu/SettingsMenu.ixx
        Code/Tasks/StartApp/StartApp.ixx
        Code/Tasks/StopApp/StopApp.ixx
        Code/Tasks/ToggleAutoSave/ToggleAutoSave.ixx
)

target_sources(GameWatchCon PRIVATE
        # Core
        Code/Core/App/App.cxx
        Code/Core/AppConfig/AppConfig.cxx
        Code/Core/AppEntryPoint/Main.cxx
        Code/Core/Console/Console.cxx
        Code/Core/GameClock/GameClock.cxx
        Code/Core/GameEntry/GameEntry.cxx
        Code/Core/GameLibrary/AccessInterfaces/ReadAccess/ReadAccess.cxx
        Code/Core/GameLibrary/AccessInterfaces/WriteAccess/WriteAccess.cxx
        Code/Core/Utils/Utils.cxx

        # Tasks
        Code/Tasks/AddNewGame/AddNewGame.cxx
        Code/Tasks/ChangeAutoSaveInterval/ChangeAutoSaveInterval.cxx
        Code/Tasks/ChangeGameTitle/ChangeGameTitle.cxx
        Code/Tasks/DeleteGame/DeleteGame.cxx
        Code/Tasks/EditGamesMenu/EditGamesMenu.cxx
        Code/Tasks/ListGames/ListGames.cxx
        Code/Tasks/MainMenu/MainMenu.cxx
        Code/Tasks/ResetGameClock/ResetGameClock.cxx
        Code/Tasks/SettingsMenu/SettingsMenu.cxx
        Code/Tasks/StartApp/StartApp.cxx
        Code/Tasks/StopApp/StopApp.cxx
        Code/Tasks/ToggleAutoSave/ToggleAutoSave.cxx
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
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(GameWatchCon PRIVATE cpr::cpr nlohmann_json::nlohmann_json)