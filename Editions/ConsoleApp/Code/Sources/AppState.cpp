#include "Pch.h"
#include "AppState.h"

[[nodiscard]] auto gw::AppState::IsAppStillRunning() const noexcept -> bool {
    return keep_app_running_.load(std::memory_order_acquire) == 1;
}

auto gw::AppState::ToggleAppRunningState() noexcept -> void {
    keep_app_running_ ^= 1;
}