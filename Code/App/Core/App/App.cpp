#include "Core/App/App.hpp"

#include "Core/Task/Data.hpp"
#include "Tasks/StartApp/StartApp.hpp"
#include "Tasks/StopApp/StopApp.hpp"

auto gw::con::core::App::Start() noexcept -> void {
    app_state_.ChangeAppRunningStatus(true);
    task_stack_.Push(std::make_unique<gw::con::tasks::StartApp>(TaskContext{app_state_, app_config_}));

    while (app_state_.GetAppRunningStatus()) {
        ExpandTask();
        ProcessTask();
    }
}

auto gw::con::core::App::ExpandTask() noexcept -> void {
    if (task_stack_.InspectTop() == nullptr) {
        task_stack_.Push(std::make_unique<gw::con::tasks::StopApp>(TaskContext{app_state_, app_config_}));
        return;
    }

    if (task_stack_.InspectTop()->GetKind() == TaskKind::Bundle) {
        task_stack_.PushReversedList(*((task_stack_.Pop())->ExpandSelf()));
    }
}

auto gw::con::core::App::ProcessTask() noexcept -> void {
    if (const auto task = task_stack_.Pop())
        task->Run();
}