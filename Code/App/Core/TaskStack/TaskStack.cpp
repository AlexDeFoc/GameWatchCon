#include "Core/TaskStack/TaskStack.hpp"

using namespace gw::con::core;

auto TaskStack::Push(std::shared_ptr<Task> new_task) noexcept -> void { tasks_.push_back(std::move(new_task)); };

auto TaskStack::Pop() noexcept -> std::shared_ptr<Task> {
    if (!tasks_.empty()) {
        auto task = tasks_.back();
        tasks_.pop_back();
        return task;
    }

    return nullptr;
}

auto TaskStack::InspectTop() const noexcept -> std::shared_ptr<Task> {
    if (!tasks_.empty())
        return tasks_.back();

    return nullptr;
}