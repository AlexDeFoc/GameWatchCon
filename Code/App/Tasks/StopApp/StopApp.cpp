#include "Tasks/StopApp/StopApp.hpp"

#include "Core/TaskType/TaskType.hpp"

using namespace gw::con::tasks;

StopApp::StopApp() noexcept : Task{core::TaskType::StopApp, core::TaskRole::StandaloneTask} {}

auto StopApp::Run(interfaces::IAppRunningStatus app_running_status) noexcept -> void {
    app_running_status = false;
}