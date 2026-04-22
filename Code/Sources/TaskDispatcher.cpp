#include "Pch.h"
#include "TaskDispatcher.h"
#include "Tasks/StartApp.h"

gw::TaskDispatcher::TaskDispatcher() : next_task_{reinterpret_cast<TaskFunc>(gw::tasks::StartApp)}, app_state_{}, console_{}, disk_manager_{console_}, app_settings_{disk_manager_}, game_library_{disk_manager_, app_settings_} {}

auto gw::TaskDispatcher::Start() -> void {
    while (app_state_.IsAppStillRunning() && next_task_ != nullptr) {
        next_task_ = reinterpret_cast<TaskFunc>(next_task_(console_, app_state_, app_settings_, game_library_));
    }
}