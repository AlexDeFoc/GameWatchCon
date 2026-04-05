#pragma once

namespace gw::con::core {
enum class TaskType : int {
    StartApp,
    StopApp,
    ListMainMenuOptions,
    RequestMainMenuOptions,
    RedirectUserBasedOnInput
};
}