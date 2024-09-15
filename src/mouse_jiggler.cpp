#include <Arduino.h>

#include "M5Cardputer.h"
#include "M5GFX.h"
#include "USBHIDMouse.h"
#include "globals.h"
#include "guiElements.h"


struct Vector2 {
  int x;
  int y;
};

Vector2 squarePattern[] = { 
    Vector2{x: 20, y: 0},
    Vector2{x: 0, y: 20},
    Vector2{x: -20, y: 0},
    Vector2{x: 0, y: -20}
};

Vector2 trianlgePattern[] = { 
    Vector2{x: 20, y: 0},
    Vector2{x: -10, y: -15},
    Vector2{x: -10, y: 15}
};

Vector2 leftRightPattern[] = { 
    Vector2{x: 20, y: 0},
    Vector2{x: -20, y: 0}
};


// visualise
int lastX = 0, lastY = 0;
unsigned long lastIndex = 0;

// mechanics
bool isJigglerActive = false;
int jiggleSpeed = 1000;

// menu
int selectedPatternIndex = 0;
unsigned int selectAction = 0;
String patternNames[] = { "Square", "Triangle", "L <-> R" };
u32_t menuItemSize = sizeof(patternNames) / sizeof(*patternNames);

// circle pattern
unsigned long currentCircleAngle = 0;


void moveMouse(M5Canvas &canvas, USBHIDMouse &USB_Mouse, int deltaX, int deltaY);
void drawMouse(M5Canvas &canvas, bool isUsbConnected, int drawX, int drawY, bool goLeft, bool goRight, bool goUp, bool goDown);


int getPatternPos(u32_t patternSize){
    return millis() / jiggleSpeed % patternSize;
}

/// @brief Act as a mouse jiggler app
/// @param canvas Canvas to draw images
/// @param USB_Mouse USB Mouse reference
void be_a_mouse_jiggler(M5Canvas &canvas, USBHIDMouse &USB_Mouse) {

    int draw_visual_mouse_x = 50;
    int draw_visual_mouse_y = 50;

    Vector2 currentPos = Vector2{ x: 0, y: 0 };
    u64_t posIndex = 0;

    if(selectedPatternIndex == 0){
        int patternSize = sizeof(squarePattern) / sizeof(*squarePattern);
        posIndex = getPatternPos(patternSize);

        currentPos = squarePattern[posIndex];
    } else if(selectedPatternIndex == 1){
        int patternSize = sizeof(trianlgePattern) / sizeof(*trianlgePattern);
        posIndex = getPatternPos(patternSize);

        currentPos = trianlgePattern[posIndex];
    } else if(selectedPatternIndex == 2){ 
        int patternSize = sizeof(leftRightPattern) / sizeof(*leftRightPattern);
        posIndex = getPatternPos(patternSize);

        currentPos = leftRightPattern[posIndex];
    }

    // canvas.drawString(String(posIndex), 5, 5);
    // canvas.drawString(String(squarePatternSize), 25, 5);
    
    if (M5Cardputer.Keyboard.isChange() || _IS_USB_STATE_UPDATED) {  // key up triggers from the main menu
        _IS_USB_STATE_UPDATED = false;

        if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            if (selectAction == 0) selectedPatternIndex --;
            if (selectAction == 1) jiggleSpeed -= 100;
        } else if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            if (selectAction == 0) selectedPatternIndex ++;
            if (selectAction == 1) jiggleSpeed += 100;
        } else if (M5Cardputer.Keyboard.isKeyPressed('<')) {
            // if (selectedPatternIndex == 0) clickDownSpeed += 1000;
            if (selectAction == 1) jiggleSpeed -= 1000;
        } else if (M5Cardputer.Keyboard.isKeyPressed('?')) {
            // if (selectedPatternIndex == 0) clickDownSpeed -= 1000;
            if (selectAction == 1) jiggleSpeed += 1000;
        } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            selectAction = 0;
        } else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            selectAction = 1;
        } else if (M5Cardputer.Keyboard.isKeyPressed(' ') || M5Cardputer.Keyboard.isKeyPressed('p')) {
            isJigglerActive = !isJigglerActive;

            if (isJigglerActive) {
                _rgbLed.setPixelColor(0, _rgbLed.Color(0, 255, 0));
            } else {
                _rgbLed.setPixelColor(0, _rgbLed.Color(255, 0, 0));
            }
            _rgbLed.show();
        }

        selectedPatternIndex = constrain(selectedPatternIndex, 0, menuItemSize-1);
        jiggleSpeed = constrain(jiggleSpeed, 100, 20000);

        drawLabeledTextBox(canvas, "Pattern", patternNames[selectedPatternIndex], 30, 5, canvas.width() - 60, canvas.fontHeight() + 4, TFT_BACKGROUND_COLOR, TFT_WHITE);

        canvas.fillTriangle(25, 5, 25, 50, 10, 27, selectAction == 0 ? selectedPatternIndex == 0 ? TFT_RED : TFT_GREEN : TFT_LIGHTGRAY);
        canvas.fillTriangle(canvas.width() - 25, 5, canvas.width() - 25, 50, canvas.width() - 10, 27, selectAction == 0 ? selectedPatternIndex == menuItemSize-1 ? TFT_RED : TFT_GREEN : TFT_LIGHTGRAY);

        // auto speedString = String(((float)jiggleSpeed)/1000);
        String speedString = String((float)jiggleSpeed / 1000, 1);
        drawVolumeBar(canvas, "Speed: " + speedString, 10, 85, 130, 20, jiggleSpeed, 20000, TFT_BACKGROUND_COLOR, TFT_WHITE, TFT_WHITE, selectAction == 1 ? TFT_DARKGREY : TFT_BACKGROUND_COLOR);

        // Force redraw mouse, overlapping clearing rect
        moveMouse(canvas, USB_Mouse, currentPos.x, currentPos.y);
    }


    if(lastIndex != posIndex){
        moveMouse(canvas, USB_Mouse, currentPos.x, currentPos.y);
        lastIndex = posIndex;
    }
}

/// @brief Move the mouse relative
/// @param canvas Canvas to draw relative mouse status
/// @param USB_Mouse USB Mouse status
/// @param deltaX mouse relative X movement
/// @param deltaY mouse relative Y movement
void moveMouse(M5Canvas &canvas, USBHIDMouse &USB_Mouse, int deltaX, int deltaY){
    bool isUsbConnected = _USB_PORT_STATUS == _USB_STATE_CONNECTED;

    drawMouse(canvas, isUsbConnected, canvas.width() - 50, 80, deltaX < 0, deltaX > 0, deltaY < 0, deltaY > 0);
    
    lastX = deltaX;
    lastY = deltaY;

    if(isJigglerActive) USB_Mouse.move(deltaX, deltaY);
}


void disableMouseJiggler() {
    isJigglerActive = false;
}

/// @brief Draw a box with the mouse status
/// @param canvas Canvas to draw
/// @param drawX X location
/// @param drawY Y location
/// @param goLeft animate move left
/// @param goRight animate move right
/// @param goUp animate move up
/// @param goDown animate move up
void drawMouse(M5Canvas &canvas, bool isUsbConnected, int drawX, int drawY, bool goLeft, bool goRight, bool goUp, bool goDown){
    
    canvas.fillRect(drawX - 27, drawY -23, 55, 68, TFT_BACKGROUND_COLOR);
    canvas.drawRect(drawX - 27, drawY -23, 55, 68, isJigglerActive ? TFT_GREEN : TFT_RED);

    if(goRight) drawX += 5;
    if(goLeft) drawX -= 5;
    if(goDown) drawY += 5;
    if(goUp) drawY -= 5;

    // move right
    if(goRight){
        canvas.drawWideLine(drawX - 17, drawY - 5, drawX - 17, drawY + 25, 2, TFT_WHITE);
        canvas.drawWideLine(drawX - 24, drawY + 2, drawX - 24, drawY + 17, 2, TFT_WHITE);
    }

    // move left
    if(goLeft){
        canvas.drawWideLine(drawX + 17, drawY - 5, drawX + 17, drawY + 25, 2, TFT_WHITE);
        canvas.drawWideLine(drawX + 24, drawY + 2, drawX + 24, drawY + 17, 2, TFT_WHITE);
    }

    // move down
    if(goDown){
        canvas.drawWideLine(drawX - 7, drawY - 16, drawX + 7, drawY - 16, 1, TFT_WHITE);
        canvas.drawWideLine(drawX - 3, drawY - 21, drawX + 3, drawY - 21, 1, TFT_WHITE);
    }

    // move up
    if(goUp){
        canvas.drawWideLine(drawX - 7, drawY + 36, drawX + 7, drawY + 36, 1, TFT_WHITE);
        canvas.drawWideLine(drawX - 3, drawY + 41, drawX + 3, drawY + 41, 1, TFT_WHITE);
    }

    canvas.drawWideLine(drawX, drawY, drawX, drawY + 20, 10, TFT_WHITE);
    canvas.fillRect(drawX - 7, drawY + 1, 16, 3, TFT_BLACK);
    canvas.fillRect(drawX , drawY - 7, 1, 6, TFT_BLACK);

    if(!isUsbConnected){
        // move offset back
        if(goRight) drawX -= 5;
        if(goLeft) drawX += 5;
        if(goDown) drawY -= 5;
        if(goUp) drawY += 5;

        // draw boxed cross
        drawCrossedBox(canvas, drawX - 22, drawY - 19, 45, 58);
    }
}