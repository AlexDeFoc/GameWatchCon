export module Core:AppStatusAccess;

export namespace gw::con::core {
class AppStatusAccess {
public:
    enum class Status : bool { Inactive,
                               Active };

    AppStatusAccess() noexcept = default;
    [[nodiscard]] auto GetStatus() const noexcept -> Status;
    auto SetStatus(Status) noexcept -> void;

private:
    Status app_status;
};
} // namespace gw::con::core