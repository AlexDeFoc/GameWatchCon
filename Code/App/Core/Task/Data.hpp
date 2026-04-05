#pragma once

#include "Core/AppConfig/AppConfig.hpp"
#include "Core/AppState/AppState.hpp"

namespace gw::con::core {
class TaskContext {
public:
    TaskContext(AppState&, AppConfig&) noexcept;
    virtual ~TaskContext() = default;

protected:
    AppState& app_state;
    AppConfig& app_config;
};
} // namespace gw::con::core