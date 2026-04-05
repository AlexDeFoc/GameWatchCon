#include "Core/TaskStack/TaskStack.hpp"

using namespace gw::con::core;

auto TaskStack::Push(std::unique_ptr<Task> new_task) noexcept -> void { tasks_.push_back(std::move(new_task)); };

auto TaskStack::Pop() noexcept -> std::unique_ptr<Task> {
    if (tasks_.empty())
        return nullptr;

    auto top = std::move(tasks_.back());
    tasks_.pop_back();
    return top;
}

auto TaskStack::InspectTop() const noexcept -> const Task* {
    if (tasks_.empty())
        return nullptr;

    return tasks_.back().get();
}