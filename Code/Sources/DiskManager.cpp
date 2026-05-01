// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "DiskManager.hpp"
#include "GamesLibraryFile.hpp"

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

auto gw::DiskManager::LoadGamesLibraryFile() const noexcept -> std::optional<std::vector<GameEntry>> {
    glz::generic file_as_json;
    std::string buffer;

    auto err = glz::read_file_json(file_as_json, games_library_file_path_, buffer);
    if (err || !file_as_json.is_object())
        return {};

    // NOTE: The following comments have been copied from 'LoadSettingsFile()'
    /* NOTE: Code to be used when migrating future fields
    std::array<std::string_view> migratable_fields = {"auto_save_enabled_status_version_older"};
    auto& file_as_json_obj = file_as_json.get_object();
    std::unordered_map<std::string_view, glz::generic> unknown_fields;
    for (const auto& [key, value] : file_as_json_obj) {
        if (!std::ranges::contains(migratable_fields, key))
            unknown_fields.insert({key, value});
    }
    */

    GamesLibraryCpyFile file{};
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

    return file.games;
}

auto gw::DiskManager::UpdateGamesLibraryFile(std::vector<GameEntry>& local_games_library) const noexcept -> void {
    GamesLibraryRefFile file{&local_games_library};
    constexpr auto pretty_opts = glz::opts{.prettify = true};
    [[maybe_unused]] auto _ = glz::write_file_json<pretty_opts>(file, games_library_file_path_, std::string{});
}

auto gw::DiskManager::ToggleAutoSaveStatus(const bool new_value) const noexcept -> void {
    SettingsFile file{};
    auto err = glz::read_file_json<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(file, settings_file_path_, std::string{});

    if (err && err != glz::error_code::file_open_failure)
        return;

    file.auto_save_enabled_status = new_value;

    constexpr auto pretty_opts = glz::opts{.prettify = true};
    [[maybe_unused]] auto _ = glz::write_file_json<pretty_opts>(file, settings_file_path_, std::string{});
}

auto gw::DiskManager::SetAutoSaveInterval(const gw::minutes new_interval) const noexcept -> void {
    SettingsFile file{};
    auto err = glz::read_file_json<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(file, settings_file_path_, std::string{});

    if (err && err != glz::error_code::file_open_failure)
        return;

    file.auto_save_interval_in_minutes = new_interval.count();

    constexpr auto pretty_opts = glz::opts{.prettify = true};
    [[maybe_unused]] auto _ = glz::write_file_json<pretty_opts>(file, settings_file_path_, std::string{});
}

auto gw::DiskManager::CreateGamesLibraryFileBackup() const noexcept -> std::optional<CreateGamesLibraryFileBackupResult> {
    using ResType = CreateGamesLibraryFileBackupResult;

    try {
        std::error_code err;

        if (!std::filesystem::exists(games_library_file_path_, err))
            return ResType::GamesLibraryFileNotFound;

        if (err)
            return ResType::UnknownError;

        std::filesystem::copy_file(games_library_file_path_, games_library_backup_file_path_, std::filesystem::copy_options::overwrite_existing, err);

        if (err)
            return ResType::UnknownError;
    } catch (...) {
        return ResType::UnknownError;
    }

    return std::nullopt;
}

// TODO: Replace all #ifdef with #if defined() in the codebase
// Private
auto gw::DiskManager::GetExeDirPath() noexcept -> std::string {
    std::filesystem::path exe_path{};

    auto safe_current_path = []() noexcept -> std::string {
        std::error_code ec;
        auto p = std::filesystem::current_path(ec);
        return ec ? std::string{"."} : p.string();
    };

    try {
#if defined(_WIN32)
        DWORD path_length_capacity = MAX_PATH;
        std::wstring wide_path{};

        do {
            wide_path.resize(path_length_capacity);
            auto actual_path_length = ::GetModuleFileNameW(nullptr, wide_path.data(), path_length_capacity);

            if (actual_path_length == 0)
                return safe_current_path();

            if (actual_path_length < path_length_capacity) {
                wide_path.resize(actual_path_length);
                break;
            }

            path_length_capacity *= 2;
        } while (true);

        auto utf8_bytes_needed = ::WideCharToMultiByte(CP_UTF8, 0, wide_path.c_str(), -1, nullptr, 0, nullptr, nullptr);

        if (utf8_bytes_needed == 0)
            return safe_current_path();

        std::string utf8_path(static_cast<std::size_t>(utf8_bytes_needed - 1), '\0'); // -1 to exclude null terminator

        auto utf8_bytes_written = ::WideCharToMultiByte(CP_UTF8, 0, wide_path.c_str(), -1, utf8_path.data(), utf8_bytes_needed, nullptr, nullptr);

        if (utf8_bytes_written == 0)
            return safe_current_path();

        exe_path = std::filesystem::path{utf8_path};
#elif defined(__linux__)
        char buf[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);

        if (len != -1) {
            buf[len] = '\0';
            exe_path = buf;
        } else {
            return safe_current_path();
        }
#elif defined(__APPLE__)
        char buf[PROC_PIDPATHINFO_MAXSIZE]; // typically 4096

        if (proc_pidpath(getpid(), buf, sizeof(buf)) > 0) {
            exe_path = buf;
        } else {
            return safe_current_path();
        }
#else
        return safe_current_path();
#endif
        // Resolve any symlinks to get the real path
        std::error_code ec;
        auto canonical = std::filesystem::canonical(exe_path, ec);
        if (!ec) {
            return canonical.parent_path().string();
        }

        // If canonical failed, fall back to the unresolved path
        return exe_path.parent_path().string();
    } catch (...) {
        return safe_current_path();
    }
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

