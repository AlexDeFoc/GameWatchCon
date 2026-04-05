#pragma once

namespace gw::con::core::app_state_interfaces {
class IAppRunningStatus {
public:
    [[nodiscard]] auto GetAppRunningStatus() const noexcept -> bool;
    auto ChangeAppRunningStatus(bool) noexcept -> void;

private:
    bool keep_app_running_;
};
}