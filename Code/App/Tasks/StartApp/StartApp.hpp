#pragma once

#include "Core/Task/Base.hpp"

namespace gw::con::tasks {
class StartApp : public core::Task {
public:
    StartApp(core::AppState&, core::AppConfig&) noexcept;
    auto Run() noexcept -> void override;
    [[nodiscard]] auto ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> override;
};
} // namespace gw::con::tasks