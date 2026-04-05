module;

#include <memory>

module Core;
import :App;
import Tasks;

using namespace gw::con::core;
using namespace gw::con::tasks;

App::App() noexcept : app_state_{}, app_config_{}, task_stack_{} {}

auto App::Start() noexcept -> void {
    app_state_.SetStatus(AppStatusAccess::Status::Active);
    task_stack_.Push(std::make_unique<StartApp>(Task::Context{app_state_, app_config_}));

    while (app_state_.GetStatus() == AppStatusAccess::Status::Active) {
        ExpandTask();
        ProcessTask();
    }
}

auto App::ExpandTask() noexcept -> void {
    if (task_stack_.InspectTop() == nullptr) {
        task_stack_.Push(std::make_unique<StopApp>(Task::Context{app_state_, app_config_}));
        return;
    }

    if (task_stack_.InspectTop()->GetKind() == Task::Kind::Bundle) {
        task_stack_.PushReversedList(*((task_stack_.Pop())->ExpandSelf()));
    }
}

auto App::ProcessTask() noexcept -> void {
    if (const auto task = task_stack_.Pop())
        task->Run();
}