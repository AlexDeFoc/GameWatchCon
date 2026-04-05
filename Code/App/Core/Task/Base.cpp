#include "Core/Task/Base.hpp"

using namespace gw::con::core;

Task::Task(TaskKind kind, std::unique_ptr<TaskContext> ctx) noexcept : kind_{kind}, ctx{std::move(ctx)} {}

auto Task::GetKind() const noexcept -> TaskKind { return kind_; }