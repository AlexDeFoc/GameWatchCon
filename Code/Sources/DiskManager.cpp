// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "DiskManager.h"

// Public
gw::DiskManager::DiskManager() : settings_file_path_{GetSettingsFilepath()}, games_library_file_path_{GetGamesLibraryFilepath()}, settings_backup_file_path_{GetSettingsBackupFilepath()}, games_library_backup_file_path_{GetGamesLibraryBackupFilepath()} {}

auto gw::DiskManager::RestoreSettingsDefaults() const noexcept -> void {
    SettingsFile file{};
    constexpr auto pretty_opts = glz::opts{.prettify = true};
    [[maybe_unused]] auto _ = glz::write_file_json<pretty_opts>(file, settings_file_path_, std::string{});
}

auto gw::DiskManager::LoadSettingsFile() const noexcept -> std::optional<SettingsFile> {
    glz::generic file_as_json;
    std::string buffer;

    auto err = glz::read_file_json(file_as_json, settings_file_path_, buffer);
    if (err || !file_as_json.is_object())
        return {};

    /* NOTE: Code to be used when migrating future fields
    std::array<std::string_view> migratable_fields = {"auto_save_enabled_status_version_older"};
    auto& file_as_json_obj = file_as_json.get_object();
    std::unordered_map<std::string_view, glz::generic> unknown_fields;
    for (const auto& [key, value] : file_as_json_obj) {
        if (!std::ranges::contains(migratable_fields, key))
            unknown_fields.insert({key, value});
    }
    */

    SettingsFile file{};
    err = glz::read<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(file, buffer);

    /* NOTE: Code to be used when migrating future fields
    if (file.file_version == 2) {
        if (unknown_fields.contains("auto_save_enabled_status_version_older")) {
            file.auto_save_enabled_status = unknown_fields.at("auto_save_enabled_status_version_older").get<bool>();
        }
    }
    */

    if (err)
        return {};

    return file;
}

auto gw::DiskManager::LoadGamesLibraryFile() const noexcept -> void {
    // GamesLibraryCopyFile file{.file_version = 1, .games{}};

    // file.games.emplace_back("Among Us", 300);

    // constexpr auto pretty_opts = glz::opts{.prettify = true};
    // auto _ = glz::write_file_json<pretty_opts>(file, games_library_file_path_, std::string{});
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::ResetAllGamesPlaytime() const noexcept -> void {
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::DeleteAllGames() const noexcept -> void {
}

auto gw::DiskManager::AddNewGame([[maybe_unused]] std::string&& game_title) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};
}

auto gw::DiskManager::ToggleAutoSaveStatus(const bool new_value) const noexcept -> void {
    SettingsFile file{};
    auto err = glz::read_file_json<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(file, settings_file_path_, std::string{});

    if (err)
        return;

    file.auto_save_enabled_status = new_value;

    constexpr auto pretty_opts = glz::opts{.prettify = true};
    [[maybe_unused]] auto _ = glz::write_file_json<pretty_opts>(file, settings_file_path_, std::string{});
}

auto gw::DiskManager::SetAutoSaveInterval(const gw::minutes new_interval) const noexcept -> void {
    SettingsFile file{};
    auto err = glz::read_file_json<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(file, settings_file_path_, std::string{});

    if (err)
        return;

    file.auto_save_interval_in_minutes = new_interval.count();

    constexpr auto pretty_opts = glz::opts{.prettify = true};
    [[maybe_unused]] auto _ = glz::write_file_json<pretty_opts>(file, settings_file_path_, std::string{});
}

// TODO: Optimize process
auto gw::DiskManager::SetGameTitle([[maybe_unused]] const int game_id, [[maybe_unused]] std::string&& game_title) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};
}

// TODO: Optimize process
auto gw::DiskManager::ResetGamePlaytime([[maybe_unused]] const int game_id) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};
}

auto gw::DiskManager::DeleteGame([[maybe_unused]] const int game_id) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};
}

auto gw::DiskManager::AddGamePlaytime([[maybe_unused]] const int game_id, [[maybe_unused]] const long long elapsed_time) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};
}

// Private
auto gw::DiskManager::GetExeDirPath() noexcept -> std::string {
#ifdef _WIN32
    DWORD path_length_capacity = MAX_PATH;
    std::wstring wide_path{};

    do {
        wide_path.resize(path_length_capacity);
        auto actual_path_length = ::GetModuleFileNameW(nullptr, wide_path.data(), path_length_capacity);

        if (actual_path_length == 0)
            return std::filesystem::current_path().string();

        if (actual_path_length < path_length_capacity) {
            wide_path.resize(actual_path_length);
            break;
        }

        path_length_capacity *= 2;
    } while (true);

    auto utf8_bytes_needed = ::WideCharToMultiByte(CP_UTF8, 0, wide_path.c_str(), -1, nullptr, 0, nullptr, nullptr);

    if (utf8_bytes_needed == 0)
        return std::filesystem::current_path().string();

    std::string utf8_path(static_cast<std::size_t>(utf8_bytes_needed - 1), '\0'); // -1 to exclude null terminator

    auto utf8_bytes_written = ::WideCharToMultiByte(CP_UTF8, 0, wide_path.c_str(), -1, utf8_path.data(), utf8_bytes_needed, nullptr, nullptr);

    if (utf8_bytes_written == 0)
        return std::filesystem::current_path().string();

    return std::filesystem::path{utf8_path}.parent_path().string();
#else
    return std::filesystem::current_path().string(); // TODO: Impl corrent way for linux & mac
#endif
}

auto gw::DiskManager::GetSettingsFilepath() noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / settings_file_name_).string();
}

auto gw::DiskManager::GetGamesLibraryFilepath() noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / games_library_file_name_).string();
}

auto gw::DiskManager::GetSettingsBackupFilepath() noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / settings_backup_file_name_).string();
}

auto gw::DiskManager::GetGamesLibraryBackupFilepath() noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / games_library_backup_file_name_).string();
}
