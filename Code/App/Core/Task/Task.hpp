#pragma once

#include "Core/TaskType/TaskType.hpp"
#include "Core/TaskRole/TaskRole.hpp"
#include "Core/TaskInterface/TaskInterface.hpp"

namespace gw::con::core {
class Task {
public:
    Task(const TaskType type, const TaskRole role) noexcept;
    virtual ~Task() = default;
    [[nodiscard]] virtual auto GetType() const noexcept -> TaskType;
    [[nodiscard]] virtual auto GetRole() const noexcept -> TaskRole;
    virtual auto Run(TaskInterface) noexcept -> void = 0;

private:
    TaskType type_;
    TaskRole role_;
};
} // namespace gw::con::core