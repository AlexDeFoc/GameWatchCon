#include "Pch.h"
#include "Tasks/StopApp.h"
#include "AppState.h"

auto gw::tasks::StopApp(gw::Console&, gw::AppState& app_state, gw::AppSettings&, gw::GameLibrary&) -> void* {
    app_state.ToggleAppRunningState();
    return nullptr;
}