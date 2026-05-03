// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (C) 2026 Sava Alexandru-Andrei
// License: GNU AGPL v3 or later - see LICENSE file

#include "Pch.hpp"
#include "Tasks/CheckForUpdates.hpp"
#include "Tasks/MainMenu.hpp"
#include "AppState.hpp"
#include "Console.hpp"
#include "RepoMetadata.hpp"

namespace {
struct VersionAsNumbers {
    std::int64_t Major{1};
    std::int64_t Minor{1};
    std::int64_t Patch{0};

    auto operator<=>(const VersionAsNumbers&) const noexcept = default;
};

[[nodiscard]] auto CanUserConnectToTheInternet() noexcept -> bool;
[[nodiscard]] auto GetVersionFromRepoAsText() noexcept -> std::string;
[[nodiscard]] auto GetChangesFromRepo(std::string_view latest_version_found) noexcept -> std::optional<std::vector<gw::AppUpdateChangeInFile>>;
[[nodiscard]] auto ExtractVersionFromText(std::string_view ver_text) noexcept -> std::optional<VersionAsNumbers>;
[[nodiscard]] auto IsThereAnUpdateAvailable(const VersionAsNumbers&, const VersionAsNumbers&) noexcept -> bool;
} // namespace

// TODO: Clean up the namings
auto gw::tasks::CheckForUpdates(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary&) -> Task {
    static VersionAsNumbers current_version{};

    console.ClearCout();
    console.WriteLine(Console::Tag::Info, "Current version: {}.{}.{}", current_version.Major, current_version.Minor, current_version.Patch);

    if (!CanUserConnectToTheInternet()) {
        console.WriteLine(Console::Tag::Error, "It looks like you're disconnected from the internet");
        return Task{gw::tasks::MainMenu};
    }

    const auto online_version_as_text = GetVersionFromRepoAsText();

    const auto online_version = ExtractVersionFromText(online_version_as_text);

    if (online_version == std::nullopt) {
        console.WriteLine(Console::Tag::Error, "Version file on the GitHub repo isn't formatted correctly");
        return Task{gw::tasks::MainMenu};
    }

    console.WriteLine(Console::Tag::Info, "Online version: {}.{}.{}", online_version->Major, online_version->Minor, online_version->Patch);

    if (IsThereAnUpdateAvailable(current_version, *online_version))
        console.WriteLine(Console::Tag::Tip, "There exist available updates");
    else
        console.WriteLine(Console::Tag::Tip, "There are no available updates");

    const auto result = GetChangesFromRepo(online_version_as_text);

    if (result == std::nullopt) {
        console.WriteLine(Console::Tag::Info, "No changes found in change log");
    } else {
        const auto& latest_changes_in_log = *result;
        const auto changes_version = latest_changes_in_log[0].version;

        std::println();
        Console::WriteLine(console.ColorText(Console::Color::Green, "Changes in version {}:", changes_version));
        for (const auto& change : latest_changes_in_log) {
            Console::WriteLine("Change type: {}", change.type);
            Console::WriteLine("Change description: {}", change.description);
            std::println();
        }
    }

    console.WriteLine(Console::Tag::Tip, "Press any key to go back");
    Console::RequestKeyPress();

    return Task{gw::tasks::MainMenu};
}

namespace {
auto CanUserConnectToTheInternet() noexcept -> bool {
    const auto response = cpr::Get(
        cpr::Url{"https://connectivitycheck.gstatic.com/generate_204"},
        cpr::Timeout{2000}, // 2 seconds waiting time
        cpr::Redirect{false} // Don't follow to a login page
    );

    return (response.status_code == 204);
}

auto GetVersionFromRepoAsText() noexcept -> std::string {
    gw::AppVersionFile file{};

    // Cache-Busting: Append random text at the end to make sure we don't get cached results from CDN
    const auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    const std::string url = "https://raw.githubusercontent.com/AlexDeFoc/GameWatchCon/main/Metadata/app_version.json?t=" + std::to_string(timestamp);
    const auto response = cpr::Get(
        cpr::Url{url},
        cpr::Header{{"User-Agent", "GameWatchCon-CheckerForUpdates"}},
        cpr::ConnectTimeout(3000), // timeout to find the server (user loses connection while connecting)
        cpr::Timeout{7000} // timeout to download file
    );

    if (response.status_code != 200 || response.error)
        return file.version;

    const auto version_as_raw_json = response.text;

    const auto err = glz::read<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(file, version_as_raw_json);
    if (err)
        return file.version;

    return file.version;
}

auto ExtractVersionFromText(const std::string_view ver_text) noexcept -> std::optional<VersionAsNumbers> {
    // clang-format off
    auto parts = ver_text | std::views::split('.')
                          | std::views::transform([](auto&& sub_range) { return std::string_view{sub_range}; })
                          | std::ranges::to<std::vector<std::string_view>>();
    // clang-format on

    if (parts.size() != 3)
        return std::nullopt;

    try {
        VersionAsNumbers v{};

        // clang-format off
        // helper - convert string_view to int
        auto to_int = [](std::string_view sv) -> int {return std::stoi(std::string{sv});};
        // clang-format on

        v.Major = to_int(parts[0]);
        v.Minor = to_int(parts[1]);
        v.Patch = to_int(parts[2]);

        return v;
    } catch (...) {
        return std::nullopt;
    }
}

auto IsThereAnUpdateAvailable(const VersionAsNumbers& current_ver, const VersionAsNumbers& online_ver) noexcept -> bool {
    bool available_update_status{false};

    if (current_ver < online_ver)
        available_update_status = true;

    return available_update_status;
}

auto GetChangesFromRepo(const std::string_view latest_version_found) noexcept -> std::optional<std::vector<gw::AppUpdateChangeInFile>> {
    // Cache-Busting: Append random text at the end to make sure we don't get cached results from CDN
    const auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    const std::string url = "https://raw.githubusercontent.com/AlexDeFoc/GameWatchCon/main/Metadata/changes_log.json?t=" + std::to_string(timestamp);
    const auto response = cpr::Get(
        cpr::Url{url},
        cpr::Header{{"User-Agent", "GameWatchCon-CheckerForUpdates"}},
        cpr::ConnectTimeout(3000), // timeout to find the server (user loses connection while connecting)
        cpr::Timeout{7000} // timeout to download file
    );

    if (response.status_code != 200 || response.error)
        return std::nullopt;

    const auto changes_log_as_raw_json = response.text;

    gw::AppChangesLogFile file{};
    const auto err = glz::read<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(file, changes_log_as_raw_json);
    if (err)
        return std::nullopt;

    // clang-format off
    auto latest_changes = file.changes | std::views::filter([&](auto&& change) { return change.version == latest_version_found; })
                                       | std::ranges::to<std::vector<gw::AppUpdateChangeInFile>>();
    // clang-format on

    if (latest_changes.empty())
        return std::nullopt;

    return latest_changes;
}
} // namespace
