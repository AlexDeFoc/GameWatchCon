#pragma once

#include "Core/TaskStack/TaskStack.hpp"

namespace gw::con::core {
// Friend declaration
class TaskInterface;

class AppState {
public:
    AppState() noexcept = default;
    auto Start() noexcept -> void;

private:
    auto ForwardTaskBasedOnRole() noexcept -> void;
    auto AddMoreTasksFromTask() noexcept -> void;
    auto ProcessTask() noexcept -> void;

    bool keep_app_running_;
    TaskStack task_stack_;

public:
    friend TaskInterface;
};
} // namespace gw::con::core