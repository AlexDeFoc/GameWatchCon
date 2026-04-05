#pragma once

#include <expected>
#include <memory>
#include <vector>
#include "Core/Task/Base.hpp"

// TODO: Might need to switch to shared_ptr for more performance (MUST benchmark)
namespace gw::con::core {
class TaskQueue {
public:
    enum class ActionStatus : int { Success,
                                    EmptyStack };
    TaskQueue() noexcept = default;
    auto Push(std::unique_ptr<Task>) noexcept -> void;
    auto PushReversedList(std::vector<std::unique_ptr<Task>>) noexcept -> void;
    [[nodiscard]] auto Pop() noexcept -> std::unique_ptr<Task>;
    [[nodiscard]] auto InspectTop() const noexcept -> const Task*;

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};
} // namespace gw::con::core