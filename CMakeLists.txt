cmake_minimum_required(VERSION 4.2)
project(GameWatchCon LANGUAGES CXX)

# Main App
add_executable(GameWatchCon)

target_sources(GameWatchCon PUBLIC FILE_SET CXX_MODULES FILES
        Code/App/Core/Console/Console.ixx
        Code/App/Core/Console/Interfaces/ConsoleComponents/ConsoleComponents.ixx
        Code/App/Core/Console/Interfaces/ConsoleWriteAccess/ConsoleWriteAccess.ixx
        Code/App/Core/Console/Interfaces/ConsoleReadAccess/ConsoleReadAccess.ixx

        Code/App/Core/TaskQueue/TaskQueue.ixx
        Code/App/Core/Task/Task.ixx

        Code/App/Core/AppConfig/AppConfig.ixx

        Code/App/Core/AppState/Interfaces/AppStatusAccess/AppStatusAccess.ixx
        Code/App/Core/AppState/AppState.ixx

        Code/App/Tasks/StartApp/StartApp.ixx
        Code/App/Tasks/StopApp/StopApp.ixx
        Code/App/Tasks/GetMainMenuOptionChoice/GetMainMenuOptionChoice.ixx
        Code/App/Tasks/ValidateMainMenuOptionChoice/ValidateMainMenuOptionChoice.ixx
        Code/App/Tasks/Tasks.ixx

        Code/App/Core/Core.ixx
        Code/App/Core/App/App.ixx
)

target_sources(GameWatchCon PRIVATE
        Code/App/Core/Console/Interfaces/ConsoleWriteAccess/ConsoleWriteAccess.cxx
        Code/App/Core/Console/Interfaces/ConsoleReadAccess/ConsoleReadAccess.cxx

        Code/App/Core/TaskQueue/TaskQueue.cxx
        Code/App/Core/Task/Task.cxx

        Code/App/Core/AppState/Interfaces/AppStatusAccess/AppStatusAccess.cxx

        Code/App/Tasks/StartApp/StartApp.cxx
        Code/App/Tasks/StopApp/StopApp.cxx
        Code/App/Tasks/GetMainMenuOptionChoice/GetMainMenuOptionChoice.cxx
        Code/App/Tasks/ValidateMainMenuOptionChoice/ValidateMainMenuOptionChoice.cxx

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