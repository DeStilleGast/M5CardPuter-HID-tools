
#include "M5GFX.h"

#include "USBHIDMouse.h"

#include "globals.h"

bool firstTickMouse = true;


void drawPad(M5Canvas &canvas, unsigned int x, unsigned int y);

void be_a_mouse(M5Canvas &canvas, USBHIDMouse &USB_Mouse){
    bool isUsbConnected = _USB_PORT_STATUS == _USB_STATE_CONNECTED;


    canvas.drawArc(60, canvas.height(), 50, 0, 180, 270, TFT_WHITE);
    canvas.drawArc(75, canvas.height(), 50, 0, 270, 360, TFT_WHITE);

    canvas.setTextColor(TFT_WHITE);
    canvas.drawString("L", 35, canvas.height() - 35);
    canvas.drawString("'", 85, canvas.height() - 35);

    canvas.drawString("Press Shift", 5, 5);
    canvas.drawString("to move", 5, 5 + canvas.fontHeight());
    canvas.drawString("faster", 5, 5 + canvas.fontHeight()*2);


    if(_IS_USB_STATE_UPDATED){
        if (isUsbConnected) {
            _rgbLed.setPixelColor(0, _rgbLed.Color(0, 255, 0));
        } else {
            _rgbLed.setPixelColor(0, _rgbLed.Color(255, 0, 0));
        }
        _rgbLed.show();
    }

    if (M5Cardputer.Keyboard.isChange() || _IS_USB_STATE_UPDATED) {  // key up triggers from the main menu
    
        _IS_USB_STATE_UPDATED = false;

        int padX = canvas.width() - 75;
        int padY = canvas.height() / 2;

        drawPad(canvas, padX, padY);

        int moveX = 0;
        int moveY = 0;

        if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            // right
            canvas.fillCircle(padX + 22, padY, 5, TFT_BLACK);
            moveX = 1;
        } else if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            // left
            canvas.fillCircle(padX - 22, padY, 5, TFT_BLACK);
            moveX = -1;
        } else if (M5Cardputer.Keyboard.isKeyPressed('?')) {
            // hard right
            canvas.fillCircle(padX + 22, padY, 5, TFT_BLACK);
            moveX = 10;
        } else if (M5Cardputer.Keyboard.isKeyPressed('<')) {
            // hard left
            canvas.fillCircle(padX - 22, padY, 5, TFT_BLACK);
            moveX = -10;
        } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            // up
            canvas.fillCircle(padX, padY - 22, 5, TFT_BLACK);
            moveY = -1;
        } else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            // down
            canvas.fillCircle(padX, padY + 22, 5, TFT_BLACK);
            moveY = 1;
        } else if (M5Cardputer.Keyboard.isKeyPressed(':')) {
            // hard up
            canvas.fillCircle(padX, padY - 22, 5, TFT_BLACK);
            moveY = -10;
        } else if (M5Cardputer.Keyboard.isKeyPressed('>')) {
            // hard down
            canvas.fillCircle(padX, padY + 22, 5, TFT_BLACK);
            moveY = 10;
        } else if (M5Cardputer.Keyboard.isKeyPressed('l')) {
            canvas.fillArc(padX - 15, padY - 15, 0, 15, 180, 270, TFT_WHITE);
            USB_Mouse.press(MOUSE_BUTTON_LEFT);
        } else if (M5Cardputer.Keyboard.isKeyPressed('\'')) {
            canvas.fillArc(padX + 14, padY - 15, 0, 15, 270, 360, TFT_WHITE);
            USB_Mouse.press(MOUSE_BUTTON_RIGHT);
        } else if (M5Cardputer.Keyboard.isKeyPressed(' ') || M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            // L click
        } else {
            USB_Mouse.release(MOUSE_BUTTON_LEFT);
            USB_Mouse.release(MOUSE_BUTTON_RIGHT);
        }

        USB_Mouse.move(moveX, moveY);
    }

}



void drawPad(M5Canvas &canvas, unsigned int x, unsigned int y){
    
    canvas.fillRect(x - 30, y - 30, 60, 30, TFT_BACKGROUND_COLOR);

    canvas.fillRect(x - 30, y - 10, 60, 20, TFT_WHITE);
    canvas.fillRect(x - 10, y - 30, 20, 60, TFT_WHITE);

    canvas.drawWideLine(x - 12, y - 12, x + 12, y + 12, 5, TFT_BACKGROUND_COLOR);
    canvas.drawWideLine(x + 12, y - 12, x - 12, y + 12, 5, TFT_BACKGROUND_COLOR);

}