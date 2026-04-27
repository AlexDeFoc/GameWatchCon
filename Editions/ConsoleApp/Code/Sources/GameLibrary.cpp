#include "Pch.h"
#include "GameLibrary.h"

gw::GameLibrary::GameLibrary(DiskManager& disk_manager, AppSettings& app_settings) : app_settings_{app_settings}, disk_manager_{disk_manager} {
    autosave_thread_ = std::jthread([this](std::stop_token stop_token) { SaveJob(stop_token); });
}

gw::GameLibrary::~GameLibrary() {
    // 1. Signal auto thread to stop
    autosave_thread_.request_stop();

    // 2. Make autosave thread wake up
    should_save_game_.store(2, std::memory_order_release); // NOTE: We use 2 to signal 'Shutdown' and not interact with the logic of 1 and 0

    // 3. Notify the thread
    should_save_game_.notify_all();

    if (app_settings_.IsAutoSaveEnabled())
        autosave_cv_.notify_all();
}

auto gw::GameLibrary::IsAnyGameActive() const noexcept -> bool {
    bool auto_save_enabled = app_settings_.IsAutoSaveEnabled();

    if (auto_save_enabled)
        return should_save_game_.load(std::memory_order_relaxed) == 1;
    else {
        bool took = took_snapshot_already_.load(std::memory_order_relaxed);
        bool pending = should_save_game_.load(std::memory_order_relaxed) == 1;

        return pending ? !took : took;
    }
}

auto gw::GameLibrary::ActiveGameTitle() const noexcept -> std::string {
    return disk_manager_.GetGameTitle(active_game_id_);
}

auto gw::GameLibrary::IsEmpty() const noexcept -> bool {
    return disk_manager_.GameCount() == 0;
}

auto gw::GameLibrary::GetGameCount() const noexcept -> int {
    return disk_manager_.GameCount();
}

auto gw::GameLibrary::GetActiveGameId() const noexcept -> int {
    return active_game_id_.load(std::memory_order_acquire);
}

auto gw::GameLibrary::GetAllGames() const noexcept -> std::vector<gw::DiskManager::GameInStorage> {
    return disk_manager_.GetAllGames();
}

auto gw::GameLibrary::AddNewGame(std::string&& game_title) const noexcept -> void {
    disk_manager_.AddNewGame(std::move(game_title));
}

// TODO: Fix params inconsistencies
auto gw::GameLibrary::SetGameTitle(int game_id, std::string&& game_title) const noexcept -> void {
    disk_manager_.SetGameTitle(game_id, std::move(game_title));
}

auto gw::GameLibrary::ResetGamePlaytime(const int game_id) const noexcept -> void {
    disk_manager_.ResetGamePlaytime(game_id);
}

auto gw::GameLibrary::DeleteGame(const int game_id) const noexcept -> void {
    disk_manager_.DeleteGame(game_id);
}

auto gw::GameLibrary::ToggleGameClock(const int game_id) noexcept -> void {
    active_game_id_.store(game_id, std::memory_order_release);

    if (!app_settings_.IsAutoSaveEnabled()) {
        should_save_game_.store(1, std::memory_order_release);
    } else {
        should_save_game_.fetch_xor(1);
    }

    should_save_game_.notify_all();
    autosave_cv_.notify_all();
}

auto gw::GameLibrary::AddGameTime(const std::chrono::steady_clock::duration time) const noexcept -> void {
    disk_manager_.AddGamePlaytime(active_game_id_.load(std::memory_order_acquire), std::chrono::duration_cast<std::chrono::seconds>(time).count());
}

auto gw::GameLibrary::SaveJob(std::stop_token stop_token) noexcept -> void {
    while (!stop_token.stop_requested()) {
        should_save_game_.wait(0, std::memory_order_acquire);

        if (stop_token.stop_requested())
            break;

        if (!app_settings_.IsAutoSaveEnabled()) {
            if (!took_snapshot_already_.load(std::memory_order_relaxed)) {
                last_snapshot_ = std::chrono::steady_clock::now();
                took_snapshot_already_.store(true, std::memory_order_release);
            } else {
                AddGameTime(std::chrono::steady_clock::now() - last_snapshot_);
                took_snapshot_already_.store(false, std::memory_order_release);
            }

            should_save_game_.store(0, std::memory_order_release);
        } else {
            last_snapshot_ = std::chrono::steady_clock::now();

            // LOOP until game is toggled OFF or the app closes
            while (!stop_token.stop_requested() && should_save_game_.load(std::memory_order_acquire) == 1) {
                std::unique_lock<std::mutex> lck{mutex_};

                bool interrupted = autosave_cv_.wait_for(lck, stop_token, app_settings_.GetAutoSaveInterval(), [&] {
                    return should_save_game_.load(std::memory_order_acquire) == 0;
                });

                auto now = std::chrono::steady_clock::now();
                AddGameTime(now - last_snapshot_);
                last_snapshot_ = now;

                if (stop_token.stop_requested() || interrupted)
                    break;
            }
        }
    }
}

auto gw::GameLibrary::GetPrintableGames(const Console& console) const noexcept -> std::string {
    // clang-format off
    const auto games = GetAllGames();
    const auto is_any_game_active = IsAnyGameActive();
    const auto active_game_id = GetActiveGameId();

    return games | std::views::transform([is_any_game_active, active_game_id, &console](auto&& game) {
        using namespace std::chrono;
        using namespace std::string_view_literals;

        auto s = seconds(game.playtime_in_sec);
        const auto d = duration_cast<days>(s); s -= d;
        const auto h = duration_cast<hours>(s); s -= h;
        const auto m = duration_cast<minutes>(s); s -= m;

        auto values = std::array<long long, 4>{d.count(), h.count(), m.count(), s.count()};
        auto labels = std::array<std::string_view, 4>{"day"sv, "h"sv, "min"sv, "s"sv};

        auto duration_str = std::views::zip(values, labels)
                          | std::views::filter([](auto&& tuple){ return std::get<0>(tuple) > 0; })
                          | std::views::transform([](auto&& tuple) {
                              auto [val, label] = tuple;
                              return std::format("{} {}{}", val, label, (label == "day" && val > 1 ? "s" : ""));
                          })
                          | std::views::join_with(" : "sv)
                          | std::ranges::to<std::string>();

        const bool is_this_game_active = is_any_game_active && (game.id == active_game_id);
        const std::string active_tag = is_this_game_active ? std::format(" - {}", console.ColorText(Console::Color::Red, "ACTIVE")) : "";

        return std::format("{}. {} - {}{}", game.id, game.title, (duration_str.empty() ? "0 s" : duration_str), active_tag);
    })           | std::views::join_with('\n')
                 | std::ranges::to<std::string>();
    // clang-format on
}

auto gw::GameLibrary::DeleteAllGames() const noexcept -> void {
    disk_manager_.DeleteAllGames();
}

auto gw::GameLibrary::ResetAllGamesPlaytime() const noexcept -> void {
    disk_manager_.ResetAllGamesPlaytime();
}

auto gw::GameLibrary::CreateGamesDatabaseBackup() const noexcept -> void {
    disk_manager_.CreateGamesDatabaseBackup();
}