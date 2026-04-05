module;

#include <memory>
#include <vector>

module Core;
import :TaskQueue;

using namespace gw::con::core;

auto TaskQueue::Push(std::unique_ptr<Task> new_task) noexcept -> void { tasks_.push_back(std::move(new_task)); };

auto TaskQueue::PushReversedList(std::vector<std::unique_ptr<Task>> new_tasks) noexcept -> void {
    for (auto it = new_tasks.rbegin(); it != new_tasks.rend(); ++it)
        tasks_.emplace_back(std::move(*it));
}

auto TaskQueue::Pop() noexcept -> std::unique_ptr<Task> {
    if (tasks_.empty())
        return nullptr;

    auto top = std::move(tasks_.back());
    tasks_.pop_back();
    return top;
}

auto TaskQueue::InspectTop() const noexcept -> const Task* {
    if (tasks_.empty())
        return nullptr;

    return tasks_.back().get();
}