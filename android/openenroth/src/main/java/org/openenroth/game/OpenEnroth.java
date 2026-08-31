package org.openenroth.game;

import android.view.KeyEvent;

import org.libsdl.app.SDLActivity;

public class OpenEnroth extends SDLActivity {
    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        // SDL's Android joystick backend deliberately maps both KEYCODE_BACK
        // and KEYCODE_BUTTON_SELECT to SDL_GAMEPAD_BUTTON_BACK. The RG405V
        // exposes these as distinct kernel inputs (KEY_BACK and BTN_SELECT),
        // so route the system Back key through SDL's keyboard path before the
        // joystick backend can merge it with Select.
        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK) {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_BACK);
                return true;
            }
            if (event.getAction() == KeyEvent.ACTION_UP) {
                SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_BACK);
                return true;
            }
        }

        return super.dispatchKeyEvent(event);
    }
}
