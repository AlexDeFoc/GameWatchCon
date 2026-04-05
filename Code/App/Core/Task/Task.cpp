#include "Core/Task/Task.hpp"

using namespace gw::con::core;

Task::Task(const TaskType type, const TaskRole role) noexcept : type_{type}, role_{role} {}

auto Task::GetType() const noexcept -> TaskType {
    return type_;
}

auto Task::GetRole() const noexcept -> TaskRole {
    return role_;
}