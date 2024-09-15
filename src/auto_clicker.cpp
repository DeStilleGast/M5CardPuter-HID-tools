#include <Arduino.h>

#include "M5Cardputer.h"
#include "M5GFX.h"
#include "USBHIDMouse.h"
#include "globals.h"
#include "guiElements.h"

// ==================================
// AUTO CLICKER
// ==================================
int32_t clickDownSpeed = 100;
int32_t clickUpSpeed = 100;
u_int16_t currentUpdateIndex = 0;
ulong lastActionMillis = 0;
bool lastActionWasDown = false;
bool isAutoClickerActive = false;

const int32_t draw_visual_mouse_x = 240 - 45;
// ==================================


/// @brief Act as a autoclicker app
/// @param canvas Canvas to draw images
/// @param USB_Mouse USB Mouse reference
void be_a_auto_clicker(M5Canvas &canvas, USBHIDMouse &USB_Mouse) {
    bool isUsbConnected = _USB_PORT_STATUS == _USB_STATE_CONNECTED;

    // canvas.fillTriangle(0, 0, 0, 50, 50, 0, isUsbConnected ? TFT_GREEN : TFT_RED);

    if (M5Cardputer.Keyboard.isChange() || _IS_USB_STATE_UPDATED) {  // key up triggers from the main menu

        if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            if (currentUpdateIndex == 0) clickDownSpeed += 100;
            if (currentUpdateIndex == 1) clickUpSpeed += 100;
        } else if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            if (currentUpdateIndex == 0) clickDownSpeed -= 100;
            if (currentUpdateIndex == 1) clickUpSpeed -= 100;
        } else if (M5Cardputer.Keyboard.isKeyPressed('?')) {
            if (currentUpdateIndex == 0) clickDownSpeed += 1000;
            if (currentUpdateIndex == 1) clickUpSpeed += 1000;
        } else if (M5Cardputer.Keyboard.isKeyPressed('<')) {
            if (currentUpdateIndex == 0) clickDownSpeed -= 1000;
            if (currentUpdateIndex == 1) clickUpSpeed -= 1000;
        } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            currentUpdateIndex = 0;
        } else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            currentUpdateIndex = 1;
        } else if (M5Cardputer.Keyboard.isKeyPressed(' ') || M5Cardputer.Keyboard.isKeyPressed('p')) {
            isAutoClickerActive = !isAutoClickerActive;

            if (isAutoClickerActive) {
                _rgbLed.setPixelColor(0, _rgbLed.Color(0, 255, 0));
            } else {
                _rgbLed.setPixelColor(0, _rgbLed.Color(255, 0, 0));
            }
            _rgbLed.show();
        }

        clickDownSpeed = constrain(clickDownSpeed, 0, 10000);
        clickUpSpeed = constrain(clickUpSpeed, 0, 10000);

        drawVolumeBar(canvas, "Down time " + String(clickDownSpeed) + "ms", 10, 10, canvas.width() - 100, 30, clickDownSpeed, 10000, TFT_BACKGROUND_COLOR, TFT_WHITE, TFT_WHITE, currentUpdateIndex == 0 ? TFT_DARKGREY : TFT_BACKGROUND_COLOR);
        drawVolumeBar(canvas, "Up time " + String(clickUpSpeed) + "ms", 10, 70, canvas.width() - 100, 30, clickUpSpeed, 10000, TFT_BACKGROUND_COLOR, TFT_WHITE, TFT_WHITE, currentUpdateIndex == 1 ? TFT_DARKGREY : TFT_BACKGROUND_COLOR);

        // clear mouse
        canvas.fillRect(draw_visual_mouse_x - 35, 9, 71, 112, TFT_BACKGROUND_COLOR);

        // mouse
        canvas.drawWedgeLine(draw_visual_mouse_x, 50, draw_visual_mouse_x, canvas.height() - 50, 30, 30, TFT_WHITE);
        canvas.fillRect(draw_visual_mouse_x - 25, 50, 51, 5, TFT_BLACK);
        canvas.fillRect(draw_visual_mouse_x - 1, 25, 3, 20, TFT_BLACK);

        // power indicator
        canvas.fillArc(draw_visual_mouse_x, canvas.height() / 2 + 15, 15, 18, 270 + 35, 270 + 325, isAutoClickerActive ? TFT_GREEN : TFT_RED);
        canvas.fillRect(draw_visual_mouse_x - 2, canvas.height() / 2 - 5, 5, 20);

        // draw crossed box if usb is not connected
        if (!isUsbConnected) {
            drawCrossedBox(canvas, draw_visual_mouse_x - 30, 15, 60, 100);
        }

        _IS_USB_STATE_UPDATED = false;
    }

    if (millis() - lastActionMillis >= (lastActionWasDown ? clickUpSpeed : clickDownSpeed)) {
        // canvas.drawString(String(millis()), 50, 50);
        lastActionMillis = millis();

        // canvas.drawCircle(20, 20, 10, lastActionWasDown ? 0xffff0000 : 0xff00ffff);

        if (isAutoClickerActive && isUsbConnected) {
            if (lastActionWasDown) {
                USB_Mouse.press();
                canvas.fillArc(draw_visual_mouse_x - 7, 44, 0, 15, 180, 270, TFT_BLACK);
            } else {
                USB_Mouse.release();
                canvas.fillArc(draw_visual_mouse_x - 7, 44, 0, 15, 180, 270, TFT_WHITE);
            }
        }

        lastActionWasDown = !lastActionWasDown;
    }
}

void disableAutoClicker() {
    isAutoClickerActive = false;
    _rgbLed.setPixelColor(0, _rgbLed.Color(0, 0, 0));
    _rgbLed.show();
    // USB_Mouse.release();
}
