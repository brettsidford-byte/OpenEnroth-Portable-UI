#include "KeyboardActionMapping.h"

#include <unordered_set>
#include <memory>

#include "Utility/MapAccess.h"

#include "InputEnumFunctions.h"
#include "InputActionContexts.h"

#ifdef __ANDROID__
#include "GUI/GUIWindow.h"
#endif

std::shared_ptr<Io::KeyboardActionMapping> keyboardActionMapping = nullptr;

extern std::unordered_set<InputAction> key_map_conflicted;  // 506E6C

namespace {
bool isGamepadKey(PlatformKey key) {
    return key >= PlatformKey::KEY_GAMEPAD_A && key <= PlatformKey::KEY_GAMEPAD_R2;
}

#ifdef __ANDROID__
PlatformKey contextualAndroidGamepadKey(InputAction action, PlatformKey configuredKey) {
    // Face buttons are deliberately reused between gameplay and individual UI
    // screens. Resolve the overlaps by screen so one physical press cannot
    // trigger both the world action and a hidden UI action.
    if (current_screen_type == SCREEN_GAME && action == INPUT_ACTION_OPEN_INVENTORY)
        return PlatformKey::KEY_NONE;

    if (current_screen_type == SCREEN_MENU) {
        if (action == INPUT_ACTION_NEW_GAME)
            return PlatformKey::KEY_NONE;
        if (action == INPUT_ACTION_OPEN_OPTIONS)
            return PlatformKey::KEY_GAMEPAD_A;
    }

    return configuredKey;
}
#endif
}

//----- (00459C8D) --------------------------------------------------------
Io::KeyboardActionMapping::KeyboardActionMapping(std::shared_ptr<GameConfig> config) : _config(config) {
    auto fill = [] (const ConfigSection &section, auto *mapping) {
        for (AnyConfigEntry *anyEntry : section.entries())
            if (KeyConfigEntry *entry = dynamic_cast<KeyConfigEntry *>(anyEntry))
                (*mapping)[entry->inputAction()] = entry;
    };

    fill(config->keybindings, &_keyboardEntryByInputAction);
    fill(config->gamepad, &_gamepadEntryByInputAction);
}

PlatformKey Io::KeyboardActionMapping::keyFor(InputAction action) const {
    KeyConfigEntry *entry = valueOr(_keyboardEntryByInputAction, action, nullptr);
    return entry ? entry->value() : PlatformKey::KEY_NONE;
}

PlatformKey Io::KeyboardActionMapping::gamepadKeyFor(InputAction action) const {
    KeyConfigEntry *entry = valueOr(_gamepadEntryByInputAction, action, nullptr);
    PlatformKey configuredKey = entry ? entry->value() : PlatformKey::KEY_NONE;
#ifdef __ANDROID__
    return contextualAndroidGamepadKey(action, configuredKey);
#else
    return configuredKey;
#endif
}

// TODO(captainurist): maybe we need to split InputActions to sets by WindowType so guarantee of only one InputAction per key is restored.
bool Io::KeyboardActionMapping::isBound(InputAction action, PlatformKey key) const {
    if (action == INPUT_ACTION_INVALID || key == PlatformKey::KEY_NONE)
        return false;
    if (KeyConfigEntry *entry = valueOr(_keyboardEntryByInputAction, action, nullptr); entry && entry->value() == key)
        return true;
    if (KeyConfigEntry *entry = valueOr(_gamepadEntryByInputAction, action, nullptr)) {
#ifdef __ANDROID__
        if (contextualAndroidGamepadKey(action, entry->value()) == key)
            return true;
#else
        if (entry->value() == key)
            return true;
#endif
    }
    return false;
}

Io::Keybindings Io::KeyboardActionMapping::currentKeybindings(KeybindingsQuery query) const {
    Io::Keybindings result;
    for (const auto &[inputAction, configEntry] : _keyboardEntryByInputAction)
        if (query == KEYBINDINGS_ALL || (query == KEYBINDINGS_CONFIGURABLE && allConfigurableInputActions().contains(inputAction)))
            result.emplace(inputAction, configEntry->value());
    return result;
}

Io::Keybindings Io::KeyboardActionMapping::defaultKeybindings(KeybindingsQuery query) const {
    Io::Keybindings result;
    for (const auto &[inputAction, configEntry] : _keyboardEntryByInputAction)
        if (query == KEYBINDINGS_ALL || (query == KEYBINDINGS_CONFIGURABLE && allConfigurableInputActions().contains(inputAction)))
            result.emplace(inputAction, configEntry->defaultValue());
    return result;
}

Io::Keybindings Io::KeyboardActionMapping::currentGamepadKeybindings(KeybindingsQuery query) const {
    Io::Keybindings result;
    for (const auto &[inputAction, configEntry] : _gamepadEntryByInputAction)
        if (query == KEYBINDINGS_ALL || (query == KEYBINDINGS_CONFIGURABLE && allConfigurableInputActions().contains(inputAction)))
            result.emplace(inputAction, configEntry->value());
    return result;
}

Io::Keybindings Io::KeyboardActionMapping::defaultGamepadKeybindings(KeybindingsQuery query) const {
    Io::Keybindings result;
    for (const auto &[inputAction, configEntry] : _gamepadEntryByInputAction)
        if (query == KEYBINDINGS_ALL || (query == KEYBINDINGS_CONFIGURABLE && allConfigurableInputActions().contains(inputAction)))
            result.emplace(inputAction, configEntry->defaultValue());
    return result;
}

void Io::KeyboardActionMapping::applyKeybindings(const Io::Keybindings &keybindings) {
    for (const auto &[inputAction, key] : keybindings) {
        KeyConfigEntry *keyboardEntry = valueOr(_keyboardEntryByInputAction, inputAction, nullptr);

        if (!isGamepadKey(key)) {
            if (keyboardEntry)
                keyboardEntry->setValue(key);
            continue;
        }

        KeyConfigEntry *gamepadEntry = valueOr(_gamepadEntryByInputAction, inputAction, nullptr);
        if (!gamepadEntry)
            continue;

        // The controls UI historically stored captured gamepad keys in the keyboard map.
        // Repair such an entry while moving the assignment into the real gamepad map.
        if (keyboardEntry && isGamepadKey(keyboardEntry->value()))
            keyboardEntry->setValue(keyboardEntry->defaultValue());

        // A physical gamepad control should belong to one configurable action. Reassigning it
        // therefore removes the old gamepad binding instead of leaving both actions active.
        for (const auto &[otherAction, otherEntry] : _gamepadEntryByInputAction) {
            if (otherAction != inputAction && otherEntry->value() == key && inputActionsShareContext(inputAction, otherAction))
                otherEntry->setValue(PlatformKey::KEY_NONE);
        }

        gamepadEntry->setValue(key);
    }
}
