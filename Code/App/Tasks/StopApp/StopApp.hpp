#pragma once

#include "Core/AppState/Interfaces/IAppRunningStatus/IAppRunningStatus.hpp"
#include "Core/Task/Base.hpp"

namespace gw::con::tasks {
class StopApp : public core::Task {
public:
    StopApp(core::TaskContext) noexcept;
    auto Run() noexcept -> void override;
    [[nodiscard]] auto ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> override;

private:
    core::app_state_interfaces::IAppRunningStatus& app_running_status_;
};
} // namespace gw::con::tasks