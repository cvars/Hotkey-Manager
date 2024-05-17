#include <map>
#include <vector>
#include <windows.h>

enum class HotkeySettings
{
    None,
    Press,
    Toggle,
    Release
};

struct HotkeyInfo {
    bool* variable;
    HotkeySettings setting;
    bool isToggled = false;
    bool isPressed = false;
    bool isReleased = false;
};

class HotkeyManager {
public:
    static HotkeyManager& getInstance() {
        static HotkeyManager instance;
        return instance;
    }

    HotkeyManager(const HotkeyManager&) = delete;
    void operator=(const HotkeyManager&) = delete;

    void registerHotkey(bool* variable, HotkeySettings setting, int key);
    void unregisterHotkey(bool* variable, int key);
    void handleSystem();

private:
    HotkeyManager() = default;
    ~HotkeyManager() = default;

    std::map<int, std::vector<HotkeyInfo>> hotkeys;

    void handleToggle(bool isPressed, HotkeyInfo& info);
    void handlePress(bool isPressed, HotkeyInfo& info);
    void handleRelease(bool isPressed, HotkeyInfo& info);
};
