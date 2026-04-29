// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.h"
#include "DiskManager.h"
#include "Console.h"

// Public
gw::DiskManager::DiskManager(Console& console) : settings_file_path_{GetSettingsFilepath()}, console_{console}, storage_backup_path_{GetStorageBackupPath()} {
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::CreateGamesDatabaseBackup() noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    // if (!std::filesystem::exists(storage_path_))
    // return;

    try {
        // std::filesystem::copy_file(storage_path_, GetStorageBackupPath(), std::filesystem::copy_options::overwrite_existing);
    } catch (...) {
        return;
    }
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::RestoreSettingsDefaults() const noexcept -> void {
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::ResetAllGamesPlaytime() const noexcept -> void {
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::DeleteAllGames() const noexcept -> void {
}

auto gw::DiskManager::GetAutoSaveEnabledStatusValue() noexcept -> long long {
    std::lock_guard<std::mutex> lck{mutex_};
    return 0;
}

auto gw::DiskManager::GetAutoSaveIntervalValue() const noexcept -> long long {
    std::lock_guard<std::mutex> lck{mutex_};


    return 0;
}

auto gw::DiskManager::AddNewGame([[maybe_unused]] std::string&& game_title) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};
}

auto gw::DiskManager::ToggleAutoSaveStatus(bool new_value) const noexcept -> void {
    SettingsFile file{};
    auto err = glz::read_file_json(file, settings_file_path_, std::string{});

    // TODO: Make sure its not the fact that the file exists but some other file opening failure like readonly or smth else!
    if (err && err.ec != glz::error_code::file_open_failure) {
        // TODO: Maybe add more ctx for the user
        return;
    }

    file.auto_save_enabled_status = new_value;

    constexpr auto pretty_opts = glz::opts{.prettify = true};
    auto write_err = glz::write_file_json<pretty_opts>(file, settings_file_path_, std::string{});

    if (write_err) {
        // TODO: Maybe add more ctx for the user
        return;
    }

    // TODO: Add context on success maybe? by returning a bool
}

auto gw::DiskManager::SetAutoSaveIntervalValue([[maybe_unused]] long long new_interval) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};
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
auto gw::DiskManager::GetExeDirPath() const noexcept -> std::string {
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

auto gw::DiskManager::GetStorageBackupPath() noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / storage_backup_filename_).string();
}

auto gw::DiskManager::GetStorageTemporaryBackupPath() noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / storage_temp_backup_filename_).string();
}

auto gw::DiskManager::GetDropDbColumnsConfirmationFilePath() const noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / drop_db_columns_confirmation_file_filename).string();
}

auto gw::DiskManager::BackupTemporarlyStorageFile() -> void {
    /*
    if (!std::filesystem::exists(storage_path_))
        return;

    try {
        std::filesystem::copy_file(storage_path_, GetStorageTemporaryBackupPath(), std::filesystem::copy_options::overwrite_existing);
    } catch (...) {
        return;
    }
    */
}

auto gw::DiskManager::RestoreTempBackupStorageFile() noexcept -> void {
    /*
    if (!std::filesystem::exists(GetStorageTemporaryBackupPath()))
        return;

    try {
        std::filesystem::remove(storage_path_);
        std::filesystem::rename(GetStorageTemporaryBackupPath(), storage_path_);

        std::ofstream confirmation_file_stream(GetDropDbColumnsConfirmationFilePath());
        if (confirmation_file_stream.is_open())
            confirmation_file_stream.close();
    } catch (...) {
        return;
    }
    */
}

auto gw::DiskManager::DeleteTempBackupStorageFile() noexcept -> void {
    try {
        if (std::filesystem::exists(GetStorageTemporaryBackupPath())) {
            std::filesystem::remove(GetStorageTemporaryBackupPath());
        }
    } catch (...) {
        return;
    }
}

auto gw::DiskManager::UserIsSureToDropColumns() const noexcept -> bool {
    try {
        if (std::filesystem::exists(GetDropDbColumnsConfirmationFilePath())) {
            std::filesystem::remove(GetDropDbColumnsConfirmationFilePath());
            return true;
        }
    } catch (...) {
        return false;
    }

    return false;
}
