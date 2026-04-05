#pragma once

#include "Core/AppState/Interfaces/IAppRunningStatus/IAppRunningStatus.hpp"
#include "Core/Task/Base.hpp"
#include "Core/Task/Data.hpp"

namespace gw::con::tasks {
class StopApp : public core::Task {
public:
    class Context : public core::TaskContext {
    public:
        Context(core::TaskContext&&) noexcept;

        core::app_state_interfaces::IAppRunningStatus& app_running_status;
    };

    StopApp(core::TaskContext) noexcept;
    auto Run() noexcept -> void override;
    [[nodiscard]] auto ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> override;

private:
    Context ctx;
};
} // namespace gw::con::tasks