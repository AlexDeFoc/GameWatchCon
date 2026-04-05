export module Core:AppState;

import :AppStatusAccess;

export namespace gw::con::core {
class AppState : public AppStatusAccess {
public:
    AppState() noexcept = default;
};
} // namespace gw::con::core