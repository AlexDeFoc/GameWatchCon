#pragma once

#include "Core/AppState/AppState.hpp"
#include "Core/AppConfig/AppConfig.hpp"

namespace gw::con::core {
class Task {
public:
    Task(AppState&, AppConfig&) noexcept;
    virtual ~Task();

protected:
    AppState& app_state;
    AppConfig& app_config;
};
} // namespace gw::con::core