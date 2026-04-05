#pragma once

#include <expected>
#include <memory>
#include <vector>
#include "Core/Task/Base.hpp"

namespace gw::con::core {
class TaskStack {
public:
    enum class ActionStatus : int { Success,
                                    EmptyStack };
    TaskStack() noexcept = default;
    auto Push(std::unique_ptr<Task>) noexcept -> void;
    [[nodiscard]] auto Pop() noexcept -> std::unique_ptr<Task>;
    [[nodiscard]] auto InspectTop() const noexcept -> const Task*;

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};
} // namespace gw::con::core