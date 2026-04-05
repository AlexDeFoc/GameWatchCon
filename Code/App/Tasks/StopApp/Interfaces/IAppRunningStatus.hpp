#pragma once

#include "Core/TaskInterface/TaskInterface.hpp"

namespace gw::con::tasks::interfaces {
class IAppRunningStatus : public core::TaskInterface {
public:
    IAppRunningStatus(core::AppState &) noexcept;

    operator bool() const;
    auto operator=(const bool) const -> void;

private:
    bool &status_;
};
} // namespace gw::con::tasks::interfaces