
#include "M5Cardputer.h"
#include "M5GFX.h"

#include "guiElements.h"

#include "USBHIDKeyboard.h"
#include "globals.h"


void drawKeyboard_random(M5Canvas &canvas, unsigned int x, unsigned int y, char pressedKey, int colorNotPushed, int colorPushed);

bool isMasherActive = false;
unsigned int mashSpeed = 1000;
unsigned long lastUpdate = 0;
unsigned long lastRelease = 0;


void be_a_keyboard_masher(M5Canvas &canvas, USBHIDKeyboard &USB_Keyboard){
    int currentUpdateIndex = 0;

    drawVolumeBar(canvas, "Speed: " + String(mashSpeed) + "ms", 5, 10, canvas.width() - 10, 30, mashSpeed, 5000, TFT_BACKGROUND_COLOR, TFT_WHITE, TFT_WHITE, currentUpdateIndex == 0 ? TFT_DARKGREY : TFT_BACKGROUND_COLOR);

    if(M5Cardputer.Keyboard.isChange()){
        if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            if (currentUpdateIndex == 0) mashSpeed += 100;
            // if (currentUpdateIndex == 1) clickUpSpeed += 100;
        } else if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            if (currentUpdateIndex == 0) mashSpeed -= 100;
            // if (currentUpdateIndex == 1) clickUpSpeed -= 100;
        } else if (M5Cardputer.Keyboard.isKeyPressed('?')) {
            if (currentUpdateIndex == 0) mashSpeed += 1000;
            // if (currentUpdateIndex == 1) clickUpSpeed += 1000;
        } else if (M5Cardputer.Keyboard.isKeyPressed('<')) {
            if (currentUpdateIndex == 0) mashSpeed -= 1000;
            // if (currentUpdateIndex == 1) clickUpSpeed -= 1000;
        } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            currentUpdateIndex = 0;
        } else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            currentUpdateIndex = 1;
        } else if (M5Cardputer.Keyboard.isKeyPressed(' ') || M5Cardputer.Keyboard.isKeyPressed('p')) {
            isMasherActive = !isMasherActive;

            if (isMasherActive) {
                _rgbLed.setPixelColor(0, _rgbLed.Color(0, 255, 0));
            } else {
                _rgbLed.setPixelColor(0, _rgbLed.Color(255, 0, 0));
            }
            _rgbLed.show();
        }

        mashSpeed = constrain(mashSpeed, 100, 5000);
    }



    if(millis() - lastRelease >= 100){
        USB_Keyboard.releaseAll();
    }

    if(millis() - lastUpdate >= mashSpeed || _IS_USB_STATE_UPDATED){
        lastUpdate = millis();

        // Generate a random key press
        int randomKey = random(0, 36); // Random number between 0 and 35
        char keyToPress;

        // Determine if the random key is a letter (a-z) or a number (0-9)
        if (randomKey < 26) {
            // For lowercase letters
            keyToPress = 'a' + randomKey; // Convert to a character (a-z)
        } else {
            // For numbers
            keyToPress = '0' + (randomKey - 26); // Convert to a number (0-9)
        }
    
        if(!isMasherActive) keyToPress = '|';

        // Serial.print("Going to press:");
        // Serial.printf("%d", randomKey);        
        // Serial.println();
        // canvas.setTextColor(TFT_WHITE);
        // canvas.fillRect(0,0, 30,30, TFT_BACKGROUND_COLOR);
        // canvas.drawString(String(keyToPress), 10, 10);


        drawKeyboard_random(canvas, canvas.width() / 2 - 73, 80, keyToPress, TFT_DARKGRAY, TFT_WHITE);

        USB_Keyboard.press(keyToPress);

        lastRelease = millis();

        _IS_USB_STATE_UPDATED = false;
    }
}

void disableKeyboardMasher(USBHIDKeyboard &USB_Keyboard){
    USB_Keyboard.releaseAll();
}

void drawKeyboard_random(M5Canvas &canvas, unsigned int x, unsigned int y, char pressedKey, int colorNotPushed, int colorPushed) {
    canvas.drawRect(x, y, 146, 46, isMasherActive ? TFT_GREEN : TFT_RED); // Outer rectangle
    // canvas.drawRect(12, 12, 104, 42, TFT_WHITE); // Inner rectangle
    
    // int pressedKey = millis() / 500 % 56;

    // Draw keys
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 14; j++) {
            KeyValue_t key = M5Cardputer.Keyboard.getKeyValue(Point2D_t{ j, i });
            
            // bool isPressed = j + i * 14 == pressedKey;
            bool isPressed = key.value_first == pressedKey;

            canvas.drawRect(x + 4 + j * 10, y + 4 + i * 10, 8, 8, isPressed ? colorPushed :colorNotPushed);
        }
    }

    if(_USB_PORT_STATUS != _USB_STATE_CONNECTED){
        drawCrossedBox(canvas, x, y, 146, 46);
    }
}
