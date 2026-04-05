cmake_minimum_required(VERSION 4.2)
project(GameWatchCon LANGUAGES CXX)

# Main App
add_executable(GameWatchCon)

target_sources(GameWatchCon PRIVATE
        Code/App/Core/AppEntryPoint/Main.cpp
        Code/App/Core/App/App.cpp
        Code/App/Core/TaskStack/TaskStack.cpp
        Code/App/Core/AppConfig/AppConfig.cpp

        Code/App/Core/Task/Base.cpp
        Code/App/Core/Task/Data.cpp

        Code/App/Core/AppState/AppState.cpp
        Code/App/Core/AppState/Interfaces/IAppRunningStatus/IAppRunningStatus.cpp

        Code/App/Tasks/StartApp/StartApp.cpp
        Code/App/Tasks/StopApp/StopApp.cpp
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