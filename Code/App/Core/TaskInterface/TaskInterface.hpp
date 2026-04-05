#pragma once

#include "Core/AppState/AppState.hpp"

namespace gw::con::core {
class TaskInterface {
public:
    explicit TaskInterface(AppState &) noexcept;
    virtual ~TaskInterface() = default;

protected:
    AppState &app_state_;

    [[nodiscard]] auto GetAppRunningStatus() const noexcept -> bool;
};
} // namespace gw::con::core