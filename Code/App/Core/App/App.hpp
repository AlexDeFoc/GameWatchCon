#pragma once

#include "Core/AppConfig/AppConfig.hpp"
#include "Core/AppState/AppState.hpp"
#include "Core/TaskQueue/TaskQueue.hpp"

namespace gw::con::core {
class App {
public:
    App() noexcept = default;
    auto Start() noexcept -> void;
    auto ExpandTask() noexcept -> void;
    auto ProcessTask() noexcept -> void;

private:
    AppState app_state_;
    AppConfig app_config_;
    TaskQueue task_stack_;
};
} // namespace gw::con::core