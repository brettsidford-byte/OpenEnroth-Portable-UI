#include "Testing/Unit/UnitTest.h"

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
