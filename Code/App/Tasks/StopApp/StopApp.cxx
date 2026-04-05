module;

#include <memory>
#include <optional>
#include <vector>

module Tasks;
import :StopApp;

using namespace gw::con::tasks;
using namespace gw::con::core;

StopApp::StopApp(Context ctx) noexcept : Task{Kind::Unit, std::move(ctx)}, app_status_{Task::ctx.app_state} {}

auto StopApp::Run() noexcept -> void { app_status_.SetStatus(AppStatusAccess::Status::Inactive); }

auto StopApp::ExpandSelf() noexcept -> std::optional<std::vector<std::unique_ptr<Task>>> { return std::nullopt; }