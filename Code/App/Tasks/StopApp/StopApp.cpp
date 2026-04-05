#include "Tasks/StopApp/StopApp.hpp"

using namespace gw::con::tasks;

StopApp::Context::Context(TaskContext&& context) noexcept : TaskContext{std::move(context)}, app_running_status{app_state} {}

StopApp::StopApp(core::TaskContext context) noexcept : Task{core::TaskKind::Unit}, ctx{std::move(context)} {}

auto StopApp::Run() noexcept -> void { ctx.app_running_status.ChangeAppRunningStatus(false); }

auto StopApp::ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> {
    return {};
}