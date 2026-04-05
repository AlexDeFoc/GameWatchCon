#include "Core/AppState/Interfaces/IAppRunningStatus/IAppRunningStatus.hpp"

using namespace gw::con::core::app_state_interfaces;

auto IAppRunningStatus::GetAppRunningStatus() const noexcept-> bool{ return keep_app_running_; }

auto IAppRunningStatus::ChangeAppRunningStatus(bool new_status) noexcept-> void{ keep_app_running_ = new_status; }