#include "Pch.h"
#include "AppSettings.h"
#include "Tasks/ToggleAutoSaveStatus.h"
#include "Tasks/SettingsMenu.h"

auto gw::tasks::ToggleAutoSaveStatus(gw::Console&, gw::AppState&, gw::AppSettings& app_settings, gw::GameLibrary&) -> void* {
    app_settings.ToggleAutoSaveStatus();
    return (void*)gw::tasks::SettingsMenu;
}