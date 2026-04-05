#include "TaskStack.hpp"

using namespace gw::con::core;

auto TaskStack::Push(Task task) noexcept -> void {
    tasks_.emplace_back(task);
}

auto TaskStack::Pop() noexcept -> Task {
    const auto top_task = tasks_.back();
    tasks_.pop_back();
    return top_task;
}

auto TaskStack::InspectTop() const noexcept -> Task {
    return tasks_.back();
}