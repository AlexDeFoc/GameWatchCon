#include "Core/Task/Base.hpp"

using namespace gw::con::core;

Task::Task(AppState& app_state, AppConfig& app_config, TaskKind kind) noexcept : app_state{app_state}, app_config{app_config}, kind_{kind} {}

auto Task::GetKind() const noexcept -> TaskKind { return kind_; }