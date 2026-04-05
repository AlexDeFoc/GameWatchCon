module;

#include <memory>
#include <optional>
#include <vector>

export module Tasks:StopApp;

import Core;

export namespace gw::con::tasks {
class StopApp : public core::Task {
public:
    explicit StopApp(Context) noexcept;
    auto Run() noexcept -> void override;
    [[nodiscard]] auto ExpandSelf() noexcept -> std::optional<std::vector<std::unique_ptr<Task>>> override;

private:
    core::AppStatusAccess& app_status_;
};
} // namespace gw::con::tasks