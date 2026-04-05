#include "Tasks/StopApp/StopApp.hpp"

using namespace gw::con::tasks;

StopApp::StopApp(core::TaskContext ctx) noexcept : Task{core::TaskKind::Unit, std::move(ctx)}, app_running_status_{Task::ctx.app_state} {}

auto StopApp::Run() noexcept -> void { app_running_status_.ChangeAppRunningStatus(false); }

auto StopApp::ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> {
    return {};
}