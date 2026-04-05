#include "Core/Task/Base.hpp"

using namespace gw::con::core;

Task::Task(TaskKind kind, TaskContext ctx) noexcept : ctx{std::move(ctx)}, kind_{kind} {}

auto Task::GetKind() const noexcept -> TaskKind { return kind_; }