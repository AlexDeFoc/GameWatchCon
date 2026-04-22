#include "Pch.h"
#include "GameLibrary.h"

gw::GameLibrary::GameLibrary(DiskManager& disk_manager, AppSettings& app_settings) : disk_manager_{disk_manager}, app_settings_{app_settings} {}

auto gw::GameLibrary::IsAnyGameActive() const noexcept -> bool {
    bool auto_save_enabled = app_settings_.IsAutoSaveEnabled();

    if (auto_save_enabled)
        return should_save_game.load(std::memory_order_relaxed);
    else {
        bool took = took_snapshot_already_.load(std::memory_order_relaxed);
        bool pending = should_save_game.load(std::memory_order_relaxed) == 1;

        return pending ? !took : took;
    }
}

auto gw::GameLibrary::ActiveGameTitle() const noexcept -> std::string {
    return disk_manager_.GetGameTitle(active_game_id_);
}

auto gw::GameLibrary::IsEmpty() const noexcept -> bool {
    return disk_manager_.GameCount() == 0;
}

auto gw::GameLibrary::GetGameCount() const noexcept -> std::size_t {
    return disk_manager_.GameCount();
}

auto gw::GameLibrary::GetActiveGameId() const noexcept -> std::size_t {
    return active_game_id_.load(std::memory_order_acquire);
}

auto gw::GameLibrary::GetAllGames() const noexcept -> std::vector<gw::DiskManager::GameInStorage> {
    return disk_manager_.GetAllGames();
}

auto gw::GameLibrary::AddNewGame(std::string&& game_title) const noexcept -> void {
    disk_manager_.AddNewGame(std::move(game_title));
}

// TODO: Fix params inconsistencies
auto gw::GameLibrary::SetGameTitle(std::size_t game_id, std::string&& game_title) const noexcept -> void {
    disk_manager_.SetGameTitle(game_id, std::move(game_title));
}

auto gw::GameLibrary::ResetGamePlaytime(const std::size_t game_id) const noexcept -> void {
    disk_manager_.ResetGamePlaytime(game_id);
}

auto gw::GameLibrary::DeleteGame(const std::size_t game_id) const noexcept -> void {
    disk_manager_.DeleteGame(game_id);
}