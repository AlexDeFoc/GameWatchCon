#pragma once

#include <memory>
#include <vector>
#include "Core/AppConfig/AppConfig.hpp"
#include "Core/AppState/AppState.hpp"
#include "Core/Task/Kind.hpp"

namespace gw::con::core {
class Task {
public:
    Task(AppState&, AppConfig&, TaskKind) noexcept;
    virtual ~Task() = default;
    virtual auto Run() noexcept -> void = 0;
    [[nodiscard]] virtual auto ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> = 0;
    [[nodiscard]] virtual auto GetKind() const noexcept -> TaskKind;

protected:
    AppState& app_state;
    AppConfig& app_config;

private:
    TaskKind kind_;
};
} // namespace gw::con::core