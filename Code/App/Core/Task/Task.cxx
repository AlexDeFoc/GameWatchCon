module;

#include <utility>

module Core;
import :Task;

using namespace gw::con::core;

Task::Context::Context(AppState& app_state, AppConfig& app_config) noexcept : app_state{app_state}, app_config{app_config} {}

Task::Task(const Kind kind, Context ctx) noexcept : ctx{std::move(ctx)}, kind_{kind} {}

auto Task::GetKind() const noexcept -> Kind { return kind_; }