#include "Pch.h"
#include "Tasks/StopApp.h"
#include "AppState.h"

auto gw::tasks::StopApp(gw::Console&, gw::AppState& app_state, gw::AppSettings&, gw::GameLibrary&) -> Task {
    app_state.ToggleAppRunningState();
    return Task{};
}