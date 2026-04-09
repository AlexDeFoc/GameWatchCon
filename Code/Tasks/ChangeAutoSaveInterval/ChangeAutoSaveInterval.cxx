module;

#include <chrono>

module Tasks;
import :ChangeAutoSaveInterval;

using namespace gw::con;

auto tasks::ChangeAutoSaveInterval(core::Console& console, core::AppConfig& app_config) noexcept -> core::TaskType {
    // TODO: Display current interval
    console.ClearScreen();
    console.RequestAutoSaveInterval();
    app_config.ChangeAutoSaveInterval(std::chrono::seconds{console.GetNumberInputResult()});
    return core::TaskType::SettingsMenu;
}