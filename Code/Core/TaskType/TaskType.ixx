export module TaskType;

export namespace gw::con::core {
enum class TaskType : int {
    Default,
    AddNewGame,
    ChangeGameTitle,
    DeleteGame,
    EditGamesMenu,
    ListGames,
    MainMenu,
    ResetGameClock,
    SettingsMenu,
    StartApp,
    StopApp,
    ToggleAutoSave
};
}