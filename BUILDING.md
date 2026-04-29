# Contributing/Developing - Tips

## Clagd intellisense

* Keep one preset always build once at least when developing. This ensures there is only one location for the file 'compile\_commands.json'

* Modify the '.clangd' file so it targets the cmake preset output path which you will primary work with. THIS doesn't imply that you cannot compile on other presets, but when you develop you should use that primary one you chose.

## Speed up CMake configuration

Run the wished preset from CMakePresets.json inside this command (replace the triplet value):
`vcpkg install --triplet=x64-windows-static-md cpr cpr[ssl] glaze glaze[ssl]`

NOTE: Run the command outside the project folder (to be able to use classic mode).

Reason: This installs the project dependencies on whole system, and when you configure the project cmake preset, vcpkg will pull the cached
binaries from the system.
