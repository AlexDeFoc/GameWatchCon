#pragma once

#include "Core/TaskType/TaskType.hpp"
#include <vector>

namespace gw::con::core {
class TaskStack {
public:
    TaskStack() noexcept = default;
    auto Push(Task) noexcept -> void;
    [[nodiscard]] auto Pop() noexcept -> Task;
    [[nodiscard]] auto InspectTop() const noexcept -> Task;

private:
    std::vector<Task> tasks_;
};
} // namespace gw::con::core