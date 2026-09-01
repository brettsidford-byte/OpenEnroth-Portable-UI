# OpenEnroth Portable UI 1.1.6

## Fixes

- Add a first `GLOBAL` page to Gamepad Controls so global actions such as Open Menu can be rebound before the Gameplay pages.
- Restore readable, bindable Arcomage actions for Play Card, Discard, Card Left and Card Right.
- While a gamepad binding is selected, replace the four small navigation controls with a full-width `CLEAR` button that assigns `KEY_NONE`; restore the normal controls after clearing or assigning the binding.
- Ignore unbound `KEY_NONE` entries when checking the controls page for duplicate bindings.
- Release all Portable Gamepad Controls button assets when leaving the screen.

Portable PNG replacements remain separate from users' original Might & Magic VII archives.
