#include "Pch.h"
#include "DiskManager.h"
#include "Console.h"

// Public
gw::DiskManager::DiskManager(Console& console) : storage_path_{GetStoragePath()}, console_{console}, storage_backup_path_{GetStorageBackupPath()}, storage_{InitStorage()} {
    using namespace sqlite_orm;

    try {
        if (!UserIsSureToDropColumns()) {
            BackupTemporarlyStorageFile();
        }

        const auto changes = storage_->sync_schema(true);

        // TODO: Use views::values
        for (const auto& [table, result] : changes) {
            switch (result) {
                case sync_schema_result::new_table_created:
                case sync_schema_result::new_columns_added:
                case sync_schema_result::dropped_and_recreated:
                case sync_schema_result::already_in_sync:
                    break;

                case sync_schema_result::old_columns_removed:
                case sync_schema_result::new_columns_added_and_old_columns_removed:
                    if (!UserIsSureToDropColumns()) {
                        RestoreTempBackupStorageFile();
                        console_.WriteLine(Console::Tag::FatalError, "Attempted to load '{}' but app was about to remove columns, thus it has crashed. Reload the program to confirm this action, else fix the database.", storage_filename_);
                        console_.WriteLine(Console::Tag::Tip, "Press any key to proceed");
                        console_.WriteLine(Console::Tag::Info, "The app will now exit");
                        console_.RequestKeyPress();
                        throw;
                    }

                    break;

                default:
                    console_.ThrowOnUnhandledCase("Unhandled sqlite-orm case");
            }
        }

        DeleteTempBackupStorageFile();

        if (storage_->pragma.user_version() == 0)
            storage_->pragma.user_version(storage_ver_);

        storage_->pragma.journal_mode(journal_mode::WAL);
        storage_->pragma.synchronous(1); // NORMAL

        if (storage_->count<SettingsInStorage>() == 0) {
            SettingsInStorage defaults{};
            storage_->insert(defaults);
        }
    } catch (const std::system_error& err) {
        if (err.code().value() == SQLITE_BUSY || err.code().value() == SQLITE_LOCKED) {
            DeleteTempBackupStorageFile();
            console_.WriteLine(Console::Tag::FatalError, "Attempted to load '{}' but found the database is in use! Open the app when the database is not in use by another process!", storage_filename_);
            console_.WriteLine(Console::Tag::Tip, "Press any key to proceed");
            console_.WriteLine(Console::Tag::Info, "The app will now exit");
            console_.RequestKeyPress();
            throw;
        } else if (err.code().value() == SQLITE_NOTADB || err.code().value() == SQLITE_CORRUPT) {
            DeleteTempBackupStorageFile();
            console_.WriteLine(Console::Tag::FatalError, "Attempted to load '{}' but found either not a valid sqlite3 database OR the database is corrupt! Open the app when you have removed the invalid database or recovered it!", storage_filename_);
            console_.WriteLine(Console::Tag::Tip, "Press any key to proceed");
            console_.WriteLine(Console::Tag::Info, "The app will now exit");
            console_.RequestKeyPress();
            throw;
        } else {
            DeleteTempBackupStorageFile();
            console_.ThrowOnUnhandledCase(std::source_location::current(), "Sqlite-orm error occured: '{}'", err.what());
        }
    } catch (const std::exception& err) {
        DeleteTempBackupStorageFile();
        console_.ThrowOnUnhandledCase(std::source_location::current(), "Unknown error occured: '{}'", err.what());
    }
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::CreateGamesDatabaseBackup() noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    if (!std::filesystem::exists(storage_path_))
        return;

    try {
        std::filesystem::copy_file(storage_path_, GetStorageBackupPath(), std::filesystem::copy_options::overwrite_existing);
    } catch (...) {
        return;
    }
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::RestoreSettingsDefaults() const noexcept -> void {
    using namespace sqlite_orm;

    std::lock_guard<std::mutex> lck{mutex_};

    try {
        storage_->update_all(set(c(&SettingsInStorage::auto_save_enabled_status) = AppSettingsDefaults::auto_save_status_default,
                                 c(&SettingsInStorage::auto_save_interval_in_min) = AppSettingsDefaults::auto_save_interval_default.count()));
    } catch (...) {
    }
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::ResetAllGamesPlaytime() const noexcept -> void {
    using namespace sqlite_orm;

    std::lock_guard<std::mutex> lck{mutex_};

    try {
        storage_->update_all(set(c(&GameInStorage::playtime_in_sec) = 0));
    } catch (...) {
    }
}

// TODO: Maybe add more context upon failure? a bool?
auto gw::DiskManager::DeleteAllGames() const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        storage_->remove_all<GameInStorage>();
    } catch (...) {
    }
}

auto gw::DiskManager::GetAutoSaveEnabledStatusValue() noexcept -> long long {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        auto result = storage_->select(&SettingsInStorage::auto_save_enabled_status, sqlite_orm::limit(1));

        if (result.empty())
            return 0;

        if (!result.empty())
            return result[0];
    } catch (...) {
        return 0;
    }

    return 0;
}

auto gw::DiskManager::GetAutoSaveIntervalValue() const noexcept -> long long {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        auto result = storage_->select(&SettingsInStorage::auto_save_interval_in_min, sqlite_orm::limit(1));

        if (!result.empty())
            return result[0];
    } catch (...) {
        return 0;
    }

    return 0;
}

// TODO: Optimize process
auto gw::DiskManager::GetGameTitle(const int game_id) const noexcept -> std::string {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        int offset_val = static_cast<int>(game_id - 1);

        auto ids = storage_->select(&GameInStorage::id,
                                    sqlite_orm::order_by(&GameInStorage::id),
                                    sqlite_orm::limit(1, sqlite_orm::offset(offset_val)));

        if (!ids.empty()) {
            auto target_id = ids.front();

            // 2. Get the actual pointer to the object
            auto game_ptr = storage_->get_pointer<GameInStorage>(target_id);

            if (game_ptr) {
                return game_ptr->title;
            }
        }
    } catch (...) {
    }

    return {};
}

auto gw::DiskManager::GameCount() const noexcept -> int {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        return storage_->count<GameInStorage>();
    } catch (...) {
    }

    return 0;
}

auto gw::DiskManager::GetAllGames() const noexcept -> std::vector<GameInStorage> {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        return storage_->get_all<GameInStorage>(sqlite_orm::order_by(&GameInStorage::id));
    } catch (...) {
    }

    return {};
}

auto gw::DiskManager::AddNewGame(std::string&& game_title) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        GameInStorage game{.title = std::move(game_title)};
        storage_->insert(game);
    } catch (...) {
    }
}

auto gw::DiskManager::ToggleAutoSaveStatus() const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    if (auto settings_ptr = storage_->get_no_throw<SettingsInStorage>(1)) {
        settings_ptr->auto_save_enabled_status = settings_ptr->auto_save_enabled_status == 1 ? 0 : 1;
        storage_->update(*settings_ptr);
    }
}

auto gw::DiskManager::SetAutoSaveIntervalValue(long long new_interval) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    if (auto settings_ptr = storage_->get_no_throw<SettingsInStorage>(1)) {
        settings_ptr->auto_save_interval_in_min = new_interval;
        storage_->update(*settings_ptr);
    }
}

// TODO: Optimize process
auto gw::DiskManager::SetGameTitle(const int game_id, std::string&& game_title) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        int offset_val = static_cast<int>(game_id - 1);

        auto ids = storage_->select(&GameInStorage::id,
                                    sqlite_orm::order_by(&GameInStorage::id),
                                    sqlite_orm::limit(1, sqlite_orm::offset(offset_val)));

        if (!ids.empty()) {
            auto target_id = ids.front();

            // 2. Get the actual pointer to the object
            auto game_ptr = storage_->get_pointer<GameInStorage>(target_id);

            if (game_ptr) {
                game_ptr->title = std::move(game_title);
                storage_->update(*game_ptr);
            }
        }
    } catch (...) {
    }
}

// TODO: Optimize process
auto gw::DiskManager::ResetGamePlaytime(const int game_id) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        int offset_val = static_cast<int>(game_id - 1);

        auto ids = storage_->select(&GameInStorage::id,
                                    sqlite_orm::order_by(&GameInStorage::id),
                                    sqlite_orm::limit(1, sqlite_orm::offset(offset_val)));

        if (!ids.empty()) {
            auto target_id = ids.front();

            // 2. Get the actual pointer to the object
            auto game_ptr = storage_->get_pointer<GameInStorage>(target_id);

            if (game_ptr) {
                game_ptr->playtime_in_sec = 0;
                storage_->update(*game_ptr);
            }
        }
    } catch (...) {
    }
}

auto gw::DiskManager::DeleteGame(const int game_id) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        int offset_val = static_cast<int>(game_id - 1);

        auto ids = storage_->select(&GameInStorage::id,
                                    sqlite_orm::order_by(&GameInStorage::id),
                                    sqlite_orm::limit(1, sqlite_orm::offset(offset_val)));

        if (!ids.empty()) {
            auto target_id = ids.front();

            // 2. Get the actual pointer to the object
            auto game_ptr = storage_->get_pointer<GameInStorage>(target_id);

            if (game_ptr) {
                storage_->remove<GameInStorage>(target_id);
            }
        }
    } catch (...) {
    }
}

auto gw::DiskManager::AddGamePlaytime(const int game_id, const long long elapsed_time) const noexcept -> void {
    std::lock_guard<std::mutex> lck{mutex_};

    try {
        int offset_val = static_cast<int>(game_id - 1);

        auto ids = storage_->select(&GameInStorage::id,
                                    sqlite_orm::order_by(&GameInStorage::id),
                                    sqlite_orm::limit(1, sqlite_orm::offset(offset_val)));

        if (!ids.empty()) {
            auto target_id = ids.front();

            // 2. Get the actual pointer to the object
            auto game_ptr = storage_->get_pointer<GameInStorage>(target_id);

            if (game_ptr) {
                game_ptr->playtime_in_sec += elapsed_time;
                storage_->update(*game_ptr);
            }
        }
    } catch (...) {
    }
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

auto gw::DiskManager::GetStoragePath() noexcept -> std::string {
    return (std::filesystem::path(GetExeDirPath()) / storage_filename_).string();
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

auto gw::DiskManager::InitStorage() -> std::unique_ptr<StorageType> {
    // clang-format off
    return std::make_unique<StorageType>(sqlite_orm::make_storage(storage_path_, sqlite_orm::make_table("settings", sqlite_orm::make_column("setting_id", &SettingsInStorage::id, sqlite_orm::primary_key()),
                                                                                                                    sqlite_orm::make_column("auto_save_enabled_status", &SettingsInStorage::auto_save_enabled_status, sqlite_orm::default_value(AppSettingsDefaults::auto_save_status_default), sqlite_orm::check(sqlite_orm::in(&SettingsInStorage::auto_save_enabled_status, {0, 1}))),
                                                                                                                    sqlite_orm::make_column("auto_save_interval_in_min", &SettingsInStorage::auto_save_interval_in_min, sqlite_orm::default_value(std::chrono::duration_cast<std::chrono::seconds>(AppSettingsDefaults::auto_save_interval_default).count()), sqlite_orm::check(sqlite_orm::c(&SettingsInStorage::auto_save_interval_in_min) >= 0))),
                                                                                 sqlite_orm::make_table("games", sqlite_orm::make_column("id", &GameInStorage::id, sqlite_orm::primary_key()),
                                                                                                                 sqlite_orm::make_column("title", &GameInStorage::title),
                                                                                                                 sqlite_orm::make_column("playtime_in_sec", &GameInStorage::playtime_in_sec, sqlite_orm::default_value(0), sqlite_orm::check(sqlite_orm::c(&GameInStorage::playtime_in_sec) >= 0)))));
    // clang-format on
}

auto gw::DiskManager::BackupTemporarlyStorageFile() -> void {
    if (!std::filesystem::exists(storage_path_))
        return;

    try {
        std::filesystem::copy_file(storage_path_, GetStorageTemporaryBackupPath(), std::filesystem::copy_options::overwrite_existing);
    } catch (...) {
        return;
    }
}

auto gw::DiskManager::RestoreTempBackupStorageFile() noexcept -> void {
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