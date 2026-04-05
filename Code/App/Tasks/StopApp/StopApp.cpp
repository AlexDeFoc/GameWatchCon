#include "Tasks/StopApp/StopApp.hpp"

using namespace gw::con::tasks;

StopApp::StopApp(core::AppState& app_state, core::AppConfig& app_config) noexcept : Task{app_state, app_config, core::TaskKind::Unit} {}

auto StopApp::Run() noexcept -> void { app_state.ChangeAppRunningStatus(false); }

auto StopApp::ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> {
    return {};
}