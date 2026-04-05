#pragma once

#include "Core/Task/Task.hpp"
#include "Tasks/StopApp/Interfaces/IAppRunningStatus.hpp"

namespace gw::con::tasks {
class StopApp : public core::Task {
public:
    StopApp() noexcept;

    auto Run(interfaces::IAppRunningStatus) noexcept -> void;
};
} // namespace gw::con::tasks