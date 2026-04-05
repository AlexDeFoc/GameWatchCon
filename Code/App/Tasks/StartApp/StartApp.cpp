#include "Tasks/StartApp/StartApp.hpp"

#include "Tasks/StopApp/StopApp.hpp"

using namespace gw::con::tasks;

StartApp::StartApp(core::TaskContext ctx) noexcept : Task{core::TaskKind::Bundle, std::move(ctx)}, app_running_status_{Task::ctx.app_state} {}

auto StartApp::Run() noexcept -> void { app_running_status_.ChangeAppRunningStatus(true); }

auto StartApp::ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> {
    return {std::make_shared<tasks::StopApp>(ctx)};
}