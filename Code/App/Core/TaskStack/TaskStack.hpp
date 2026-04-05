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
    auto Push(std::shared_ptr<Task>) noexcept -> void;
    [[nodiscard]] auto Pop() noexcept -> std::shared_ptr<Task>;
    [[nodiscard]] auto InspectTop() const noexcept -> std::shared_ptr<Task>;

private:
    std::vector<std::shared_ptr<Task>> tasks_;
};
} // namespace gw::con::core