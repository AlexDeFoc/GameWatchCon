# What to contribute with?
Whatever you want, just keep it useful or whatever improves the app or the coding experience.

# Speeding up repeted CMake configuration of presets

*NOTE: Replace the `$` symbol with the wanted preset name*

*NOTE 2: To find the available presets easily, run `cmake --preset a`*

*IMPORTANT NOTE: Run the command outside the project folder - This will install the libraries on your system, so that when you configure other presets later or reconfigure presets, it will use the cached libraries*

`vcpkg install --triplet=x64-windows-static-md cpr cpr[ssl] glaze glaze[ssl]`

*NOTE 3: The ssl extensions might not be needed, I haven't tested compiling the program without them*

# Clagd intellisense (optional)
## The following tips are useful only for those that use clangd as their intellisense engine/lsp

1. Keep one preset always build once at least when developing. This ensures there is only one location for the file `compile_commands.json`
2. Modify the `.clangd` file so it targets the cmake preset output path which you will primary work with.

*NOTE: THIS doesn't imply that you cannot compile the other presets, but when you develop/code you should always have one preset built (compiled) and which is being targeted by the `.clangd` file*
