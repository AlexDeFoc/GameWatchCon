#pragma once

#include <memory>
#include <vector>
#include "Core/Task/Data.hpp"
#include "Core/Task/Kind.hpp"

namespace gw::con::core {
class Task {
public:
    Task(TaskKind, TaskContext) noexcept;
    virtual ~Task() = default;
    virtual auto Run() noexcept -> void = 0;
    [[nodiscard]] virtual auto ExpandSelf() const noexcept -> std::vector<std::shared_ptr<Task>> = 0;
    [[nodiscard]] virtual auto GetKind() const noexcept -> TaskKind;

protected:
    TaskContext ctx;

private:
    TaskKind kind_;
};
} // namespace gw::con::core