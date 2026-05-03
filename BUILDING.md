# Dependencies
* CMake - at least `3.6`
* Compatible toolchains/compilers that can compiler c++23 code -> [Toolchains section](#supported-toolchains)
* Vcpkg - at least `2026-04-08-e0612b42ce44e55a0e630f2ee9d3c533a63d8bc1`

# Supported toolchains
## Windows
* MSVC - Visual Studio 2026 (may work with older version, but you must add the cmake presets yourself, and i haven't tried with older versions)

* ClangCL - at least `Version 22.1.4` (may work with older versions, but i haven't tried)
    * Ninja - With clang cl you wil need ninja - at least `Version 1.13.2` (may work with older versions, but i haven't tried)

* Clang - at least `Version 21.1.8` (may work with older versions, but i haven't tried)
    * Ninja - With clang you wil need ninja - at least `Version 1.13.2` (may work with older versions, but i haven't tried)

## Linux
* Clang - `Version 21.1.8` (because i haven't tested on the platform)
    * Ninja - With clang you wil need ninja - at least `Version 1.13.1` (may work with older versions, but i haven't tried)

## MacOS
* Clang - `(unknown version)` (because i haven't tested on the platform)
    * Ninja - With clang you wil need ninja - at least `(unknown version)`

*NOTE: If you want more toolchains you will need to add them to the 'CMakePresets.json' file!*

# Steps to build
* Clone the repository
* Go into the GameWatchCon directory
* Run the following commands (replace $ with whatever preset you want):
    * `cmake --preset a` <- this will list all available presets (optional)
    * `cmake --preset '$'`
    * `cmake --build --preset '$'`

*NOTE: The output binary is in the `Output/` folder
