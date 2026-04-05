export module Core:App;

import :AppState;
import :AppConfig;
import :TaskQueue;

export namespace gw::con::core {
class App {
public:
    App() noexcept;
    auto Start() noexcept -> void;
    auto ExpandTask() noexcept -> void;
    auto ProcessTask() noexcept -> void;

private:
    AppState app_state_;
    AppConfig app_config_;
    TaskQueue task_stack_;
};
} // namespace gw::con::core