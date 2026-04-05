#include "Core/Task/Data.hpp"

using namespace gw::con::core;

TaskContext::TaskContext(AppState& app_state, AppConfig& app_config) noexcept : app_state{app_state}, app_config{app_config} {}