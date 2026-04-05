module;

#include <memory>
#include <optional>
#include <vector>

module Tasks;
import :StartApp;
import :StopApp;

using namespace gw::con::tasks;
using namespace gw::con::core;

StartApp::StartApp(Context ctx) noexcept : Task{Kind::Bundle, std::move(ctx)}, app_status_{Task::ctx.app_state} {}

auto StartApp::Run() noexcept -> void { app_status_.SetStatus(AppStatusAccess::Status::Active); }

auto StartApp::ExpandSelf() noexcept -> std::optional<std::vector<std::unique_ptr<Task>>> {
    std::vector<std::unique_ptr<Task>> bundle{};
    bundle.emplace_back(std::make_unique<StopApp>(std::move(ctx)));
    return bundle;
}