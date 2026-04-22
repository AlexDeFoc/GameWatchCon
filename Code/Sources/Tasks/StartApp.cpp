#include "Pch.h"
#include "Tasks/StartApp.h"
#include "Tasks/MainMenu.h"

auto gw::tasks::StartApp(gw::Console&, gw::AppState&, gw::AppSettings&, gw::GameLibrary&) -> void* {
    return (void*)gw::tasks::MainMenu;
}