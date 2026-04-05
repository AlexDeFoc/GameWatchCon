module;

#include <memory>
#include <vector>

export module Core:TaskQueue;

import :Task;

export namespace gw::con::core {
class TaskQueue {
public:
    TaskQueue() noexcept = default;
    auto Push(std::unique_ptr<Task>) noexcept -> void;
    auto PushReversedList(std::vector<std::unique_ptr<Task>>) noexcept -> void;
    [[nodiscard]] auto Pop() noexcept -> std::unique_ptr<Task>;
    [[nodiscard]] auto InspectTop() const noexcept -> const Task*;

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};
} // namespace gw::con::core