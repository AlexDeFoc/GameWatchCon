#include "Tasks/StartApp/StartApp.hpp"

#include "Tasks/StopApp/StopApp.hpp"

using namespace gw::con::tasks;

StartApp::StartApp(core::AppState& app_state, core::AppConfig& app_config) noexcept : Task{app_state, app_config, core::TaskKind::Bundle} {}

auto StartApp::Run() noexcept -> void { app_state.ChangeAppRunningStatus(true); }

auto StartApp::ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> {
    return {};
    // return {std::make_shared<tasks::StopApp>()};
}