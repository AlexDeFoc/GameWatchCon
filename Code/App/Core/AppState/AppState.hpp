#pragma once

#include "Interfaces/IAppRunningStatus/IAppRunningStatus.hpp"

namespace gw::con::core {
class AppState : public app_state_interfaces::IAppRunningStatus {
public:
    AppState() noexcept = default;
};
}