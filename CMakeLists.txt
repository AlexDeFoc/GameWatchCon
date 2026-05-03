cmake_minimum_required(VERSION 4.3)
project(GameWatch LANGUAGES CXX)

# GameWatch - Console Edition
add_executable(GameWatchCon)

target_precompile_headers(GameWatchCon PRIVATE "Code/Includes/Pch.hpp")

file(GLOB_RECURSE GW_CPP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Code/Sources/*.cpp")

target_sources(GameWatchCon PRIVATE "${GW_CPP_SOURCES}")

target_include_directories(GameWatchCon PRIVATE Code/Includes)

set_target_properties(GameWatchCon PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if (MSVC AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(GameWatchCon PRIVATE /Wall /WX /permissive- /analyze-)
    target_compile_options(GameWatchCon PRIVATE
        /wd4820 # -Wno-padded
        /wd4061 # -Wno-covered-switch-default
        /wd4710 # - Compiler wanted to inline but the functio was too complex and abandoned the idea
        /wd4711 # - Compiler is bragging about the it inlined a function while i the dev didnt inline the function
        /wd5045 # - Compiler is saying: it would insert mitigations for a specific memory load if i the dev would have enabled /Qspectre
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC)
    target_compile_options(GameWatchCon PRIVATE -Wall -Wextra -Wpedantic -Werror -Wdeprecated -Wunsafe-buffer-usage)
    target_compile_options(GameWatchCon PRIVATE
        -Wno-unused-command-line-argument
        -Wno-c++98-compat
        -Wno-header-hygiene
        -Wno-documentation
        -Wno-newline-eof
        -Wno-c++98-compat-pedantic
        -Wno-padded
        -Wno-covered-switch-default
        -Wno-switch-enum
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang
    target_compile_options(GameWatchCon PRIVATE -Wall -Wextra -Wpedantic -Werror -Wdeprecated -fsafe-buffer-usage-suggestions -Wunsafe-buffer-usage -fcolor-diagnostics)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC
    target_compile_options(GameWatchCon PRIVATE -Wall -Wextra -Wpedantic -Werror -Wdeprecated -fsafe-buffer-usage-suggestions -Wunsafe-buffer-usage)
endif()

find_package(cpr REQUIRED)
find_package(glaze REQUIRED)
target_link_libraries(GameWatchCon PRIVATE cpr::cpr glaze::glaze)
