#pragma once

#include "Core/AppConfig/AppConfig.hpp"
#include "Core/AppState/AppState.hpp"

namespace gw::con::core {
struct TaskContext {
    TaskContext(AppState&, AppConfig&) noexcept;

    AppState& app_state;
    AppConfig& app_config;
};
} // namespace gw::con::core