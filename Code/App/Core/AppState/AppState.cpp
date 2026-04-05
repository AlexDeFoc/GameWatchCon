#include "AppState.hpp"

#include "Tasks/StopApp/StopApp.hpp"

using namespace gw::con::core;

auto AppState::Start() noexcept -> void {
    keep_app_running_ = true;
    task_stack_.Push(Task::StartApp);

    while (keep_app_running_) {
        ForwardTaskBasedOnRole();
    }
}

auto AppState::ForwardTaskBasedOnRole() noexcept -> void {
    switch (task_stack_.InspectTop()) {
        case Task::StartApp:
            AddMoreTasksFromTask();
            break;

        case Task::StopApp:
            ProcessTask();
            break;
    }
}

auto AppState::AddMoreTasksFromTask() noexcept -> void {
    switch (task_stack_.InspectTop()) {
        case Task::StartApp:
            task_stack_.Push(Task::RedirectUserBasedOnInput);
            task_stack_.Push(Task::RequestMainMenuOptions);
            task_stack_.Push(Task::ListMainMenuOptions);
            task_stack_.Push(Task::StopApp); // TODO: Remove
            break;
    }
}

auto AppState::ProcessTask() noexcept -> void {
    switch (task_stack_.InspectTop()) {
        case Task::StopApp:
            gw::con::tasks::StopApp::Run(keep_app_running_);
            break;
    }
}