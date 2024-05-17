#include "HotkeyManager.hpp"


void HotkeyManager::registerHotkey(bool* variable, HotkeySettings setting, int key)
{
    HotkeyInfo hotkeyInfo{ variable, setting };
    hotkeys[key].push_back(hotkeyInfo);
}

void HotkeyManager::unregisterHotkey(bool* variable, int key)
{
    auto& infoVector = hotkeys[key];
    infoVector.erase(
        std::remove_if(
            infoVector.begin(),
            infoVector.end(),
            [&](const HotkeyInfo& info) { return info.variable == variable; }
        ),
        infoVector.end()
    );

    // If there's no more hotkey info for this key, we can remove the key from the map.
    if (infoVector.empty()) {
        hotkeys.erase(key);
    }
}

void HotkeyManager::handleSystem()
{
    for (auto& [key, infoVector] : hotkeys)
    {
        bool isPressed = GetAsyncKeyState(key) & 0x8000;

        for (auto& info : infoVector)
        {
            switch (info.setting)
            {
            case HotkeySettings::None:
                // Do nothing
                break;

            case HotkeySettings::Toggle:
                handleToggle(isPressed, info);
                break;

            case HotkeySettings::Press:
                handlePress(isPressed, info);
                break;

            case HotkeySettings::Release:
                handleRelease(isPressed, info);
                break;
            }
        }
    }
}

void HotkeyManager::handleToggle(bool isPressed, HotkeyInfo& info)
{
    if (isPressed)
    {
        if (!info.isToggled)
        {
            *info.variable = !*info.variable;
            info.isToggled = true;
        }
    }
    else
    {
        info.isToggled = false;
    }
}

void HotkeyManager::handlePress(bool isPressed, HotkeyInfo& info)
{
    if (isPressed && !info.isPressed)
    {
        *info.variable = true;
        info.isPressed = true;
    }
    else if (!isPressed && info.isPressed)
    {
        *info.variable = false;
        info.isPressed = false;
    }
}

void HotkeyManager::handleRelease(bool isPressed, HotkeyInfo& info)
{
    if (!isPressed && !info.isReleased)
    {
        *info.variable = true;
        info.isReleased = true;
    }
    else if (isPressed && info.isReleased)
    {
        *info.variable = false;
        info.isReleased = false;
    }
}
