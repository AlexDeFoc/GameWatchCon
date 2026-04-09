export module Tasks:ChangeAutoSaveInterval;
import TaskType;
import Console;
import AppConfig;

export namespace gw::con::tasks {
[[nodiscard]] auto ChangeAutoSaveInterval(core::Console&, core::AppConfig&) noexcept -> core::TaskType;
}