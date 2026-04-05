#include "Tasks/StopApp/Interfaces/IAppRunningStatus.hpp"

using namespace gw::con::tasks::interfaces;

IAppRunningStatus::IAppRunningStatus(core::AppState &app_state) noexcept : TaskInterface{app_state}, status_{app_state_.} {}

IAppRunningStatus::operator bool() const {
    return status_;
}

auto IAppRunningStatus::operator=(const bool new_value) const -> void {
    status_ = new_value;
}