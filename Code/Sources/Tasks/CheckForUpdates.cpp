#include "Pch.h"
#include "Tasks/CheckForUpdates.h"
#include "Tasks/MainMenu.h"
#include "AppState.h"
#include "Console.h"

// TODO: Make everything cleaner

namespace {
struct Version {
    int Major;
    int Minor;
    int Patch;

    auto operator<=>(const Version&) const noexcept = default;
};

[[nodiscard]] auto CanUserConnectToTheInternet() noexcept -> bool;
[[nodiscard]] auto GetVersionFromRepoAsText() noexcept -> std::optional<std::string>;
[[nodiscard]] auto ExtractVersionFromText(std::string_view ver_text) noexcept -> std::optional<Version>;
[[nodiscard]] auto IsThereAnUpdateAvailable(const Version&, const Version&) noexcept -> bool;
} // namespace

auto gw::tasks::CheckForUpdates(gw::Console& console, gw::AppState&, gw::AppSettings&, gw::GameLibrary&) -> void* {
    static Version current_version{.Major = 1, .Minor = 0, .Patch = 0};

    console.ClearCout();
    console.WriteLineToCache(Console::Tag::Info, "Current version: {}.{}.{}", current_version.Major, current_version.Minor, current_version.Patch);

    if (!CanUserConnectToTheInternet()) {
        console.WriteLineToCache(Console::Tag::Error, "It looks like you're disconnected from the internet");
        return (void*)gw::tasks::MainMenu;
    }

    const auto online_version_as_text = GetVersionFromRepoAsText();

    if (online_version_as_text == std::nullopt) {
        console.WriteLineToCache(Console::Tag::Error, "Something went wrong while connecting to the repository from GitHub");
        return (void*)gw::tasks::MainMenu;
    }

    const auto online_version = ExtractVersionFromText(*online_version_as_text);

    if (online_version == std::nullopt) {
        console.WriteLineToCache(Console::Tag::Error, "Version file on the GitHub repo isn't formatted correctly");
        return (void*)gw::tasks::MainMenu;
    }

    console.WriteLineToCache(Console::Tag::Info, "Online version: {}.{}.{}", online_version->Major, online_version->Minor, online_version->Patch);

    if (IsThereAnUpdateAvailable(current_version, *online_version))
        console.WriteLineToCache(Console::Tag::Tip, "There exist available updates");
    else
        console.WriteLineToCache(Console::Tag::Tip, "There are no available updates");

    return (void*)gw::tasks::MainMenu;
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

auto GetVersionFromRepoAsText() noexcept -> std::optional<std::string> {
    // Cache-Busting: Append random text at the end to make sure we don't get cached results from CDN
    const auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    const std::string url = "https://raw.githubusercontent.com/AlexDeFoc/GameWatchCon/main/current_app_version.txt?t=" + std::to_string(timestamp);
    const auto response = cpr::Get(
        cpr::Url{url},
        cpr::Header{{"User-Agent", "GameWatchCon-App-VersionCheckerForUpdates"}},
        cpr::ConnectTimeout(3000), // timeout to find the server (user loses connection while connecting)
        cpr::Timeout{7000} // timeout to download file
    );

    if (response.status_code != 200 || response.error)
        return std::nullopt; // TODO: maybe check each error code? to give the user more context

    auto cleaned_version = response.text;
    cleaned_version.erase(std::ranges::remove_if(cleaned_version, ::isspace).begin(), cleaned_version.end());
    return cleaned_version;
}

auto ExtractVersionFromText(const std::string_view ver_text) noexcept -> std::optional<Version> {
    Version v{};
    const auto start = ver_text.data();
    const auto end = ver_text.data() + ver_text.size();

    auto [ptr1, ec1] = std::from_chars(start, end, v.Major);
    if (ec1 != std::errc{} || ptr1 == end || *ptr1 != '.') {
        return std::nullopt;
    }

    auto [ptr2, ec2] = std::from_chars(ptr1 + 1, end, v.Minor);
    if (ec2 != std::errc{} || ptr2 == end || *ptr2 != '.') {
        return std::nullopt;
    }

    auto [ptr3, ec3] = std::from_chars(ptr2 + 1, end, v.Patch);
    if (ec3 != std::errc{}) {
        return std::nullopt;
    }

    // Trailing junk after the 'patch' part
    if (ptr3 != end)
        return std::nullopt;

    return v;
}

auto IsThereAnUpdateAvailable(const Version& current_ver, const Version& online_ver) noexcept -> bool {
    bool available_update_status{false};

    if (current_ver < online_ver)
        available_update_status = true;

    return available_update_status;
}
} // namespace