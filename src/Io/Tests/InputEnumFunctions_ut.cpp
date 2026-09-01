#include "Testing/Unit/UnitTest.h"

#include "Io/InputActionContexts.h"
#include "Io/InputEnumFunctions.h"

UNIT_TEST(InputEnumFunctions, EveryActionHasDisplayName) {
    for (InputAction action : allInputActions())
        EXPECT_NE(GetDisplayName(action), "-INVALID-ACTION-");
}

UNIT_TEST(InputEnumFunctions, ArcomageDisplayNamesFitControlsMenu) {
    EXPECT_EQ(GetDisplayName(INPUT_ACTION_ARCOMAGE_PLAY_CARD), "PLAY CARD");
    EXPECT_EQ(GetDisplayName(INPUT_ACTION_ARCOMAGE_DISCARD), "DISCARD");
    EXPECT_EQ(GetDisplayName(INPUT_ACTION_ARCOMAGE_LEFT), "CARD LEFT");
    EXPECT_EQ(GetDisplayName(INPUT_ACTION_ARCOMAGE_RIGHT), "CARD RIGHT");
}

UNIT_TEST(InputActionContexts, GlobalPageComesFirst) {
    EXPECT_STREQ(kInputActionPages[0].title, "GLOBAL");
    EXPECT_EQ(kInputActionPages[0].context, InputActionContext::Global);
    EXPECT_EQ(kInputActionPages[1].context, InputActionContext::Gameplay);

    EXPECT_EQ(GetDisplayName(INPUT_ACTION_ESCAPE), "OPEN MENU");
    EXPECT_EQ(inputActionContext(INPUT_ACTION_ESCAPE), InputActionContext::Global);
    EXPECT_EQ(inputActionContext(INPUT_ACTION_TAKE_SCREENSHOT), InputActionContext::Global);
    EXPECT_EQ(inputActionContext(INPUT_ACTION_OPEN_CONSOLE), InputActionContext::Global);
    EXPECT_EQ(inputActionContext(INPUT_ACTION_TOGGLE_WINDOW_MODE), InputActionContext::Global);
}

UNIT_TEST(InputActionContexts, NoneBindingsDoNotConflict) {
    std::unordered_map<InputAction, PlatformKey> bindings = {
        {INPUT_ACTION_MOVE_FORWARD, PlatformKey::KEY_NONE},
        {INPUT_ACTION_MOVE_BACKWARDS, PlatformKey::KEY_NONE},
        {INPUT_ACTION_JUMP, PlatformKey::KEY_GAMEPAD_A},
        {INPUT_ACTION_ATTACK, PlatformKey::KEY_GAMEPAD_A},
    };

    std::unordered_set<InputAction> conflicts = conflictingInputActions(bindings);
    EXPECT_EQ(conflicts.size(), 2);
    EXPECT_TRUE(conflicts.contains(INPUT_ACTION_JUMP));
    EXPECT_TRUE(conflicts.contains(INPUT_ACTION_ATTACK));
    EXPECT_FALSE(conflicts.contains(INPUT_ACTION_MOVE_FORWARD));
    EXPECT_FALSE(conflicts.contains(INPUT_ACTION_MOVE_BACKWARDS));
}
