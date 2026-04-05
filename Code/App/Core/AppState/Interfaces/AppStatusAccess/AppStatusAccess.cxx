module Core;
import :AppStatusAccess;

using namespace gw::con::core;

auto AppStatusAccess::GetStatus() const noexcept -> Status { return app_status; }

auto AppStatusAccess::SetStatus(const Status new_status) noexcept -> void { app_status = new_status; }