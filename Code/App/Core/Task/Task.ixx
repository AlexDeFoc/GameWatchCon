module;
#include <memory>
#include <optional>
#include <vector>

export module Core:Task;

import :AppState;
import :AppConfig;

export namespace gw::con::core {
class Task {
public:
    enum class Kind { Bundle,
                      Unit };

    struct Context {
        Context(AppState&, AppConfig&) noexcept;

        AppState& app_state;
        AppConfig& app_config;
    };

    Task(Kind, Context) noexcept;
    virtual ~Task() noexcept = default;
    virtual auto Run() noexcept -> void = 0;
    [[nodiscard]] virtual auto ExpandSelf() noexcept -> std::optional<std::vector<std::unique_ptr<Task>>> = 0;
    [[nodiscard]] virtual auto GetKind() const noexcept -> Kind;

protected:
    Context ctx;

private:
    Kind kind_;
};
} // namespace gw::con::core