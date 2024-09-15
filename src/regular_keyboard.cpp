#include "M5GFX.h"
#include "M5Cardputer.h"
#include "USBHIDKeyboard.h"

#include "globals.h"
#include "guiElements.h"


void be_a_keyboard(M5Canvas &canvas, USBHIDKeyboard &USB_Keyboard){

    canvas.setTextColor(_USB_PORT_STATUS == _USB_STATE_CONNECTED ? TFT_GREEN : TFT_RED);
    canvas.drawCenterString("State: Unknown", canvas.width() / 2, 20);

    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            // for (auto i : status.word) {
            //     Keyboard.press(i);
            // }
            KeyReport report = {0};
            report.modifiers = status.modifiers;
            uint8_t index = 0;
            for (auto i : status.hid_keys) {
                report.keys[index] = i;
                index++;
                if (index > 5) {
                    index = 5;
                }
            }
            USB_Keyboard.sendReport(&report);
            USB_Keyboard.releaseAll();

            // only text for display
            String keyStr = "";
            for (auto i : status.word) {
                if (keyStr != "") {
                    keyStr = keyStr + "+" + i;
                } else {
                    keyStr += i;
                }
            }

            // if (keyStr.length() > 0) {
            //     canvas.drawString(keyStr, canvas.width() / 2, canvas.height() / 2);
            // }
        } else {
            // canvas.drawString("USB Keyboard", canvas.width() / 2, canvas.height() / 2);
        }
    }

    drawKeyboard(canvas, canvas.width() / 2 - 73, 80, TFT_DARKGRAY, TFT_WHITE);

}

void disableRegularKeyboard(){

}