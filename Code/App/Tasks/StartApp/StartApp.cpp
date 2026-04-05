#include "Tasks/StartApp/StartApp.hpp"

#include "Tasks/StopApp/StopApp.hpp"

using namespace gw::con::tasks;

StartApp::Context::Context(TaskContext&& context) noexcept : TaskContext{std::move(context)}, app_running_status{app_state} {}

StartApp::StartApp(core::TaskContext context) noexcept : Task{core::TaskKind::Bundle, std::make_unique<Context>(std::move(context))} {}

auto StartApp::Run() noexcept -> void { (std::reinterpret_pointer_cast<Context>(ctx))->app_running_status.ChangeAppRunningStatus(true); }

auto StartApp::ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> {
    return {};
    // return {std::make_shared<tasks::StopApp>()};
}