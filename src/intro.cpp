#include "M5Cardputer.h"
#include "M5GFX.h"

void runIntro(M5Canvas &c){

    c.clear();

    c.drawLine(20, c.height() - 50, c.width() - 40, c.height() - 50, TFT_WHITE);
    c.drawLine(c.width() - 40, 10, c.width() - 40, c.height() - 50, TFT_WHITE);

    for(int keyX = 0; keyX < 7; keyX++){
        for(int keyY = 0; keyY < 4; keyY++){
            if(keyX == 6 && keyY == 3){
                c.fillRect(25 + keyX * 25, 1 + keyY * 20, 20, 15, TFT_GREEN);
            } else { 
                c.drawRect(25 + keyX * 25, 1 + keyY * 20, 20, 15, TFT_WHITE);
            }
        }
    }

    c.drawCenterString("Press [space] to", c.width() / 2, c.height() - 40, &fonts::FreeMono9pt7b);
    c.drawCenterString("toggle activation", c.width() / 2, c.height() - 20, &fonts::FreeMono9pt7b);

    c.pushSprite(0, 0);

    
    while(true){
        M5Cardputer.update();

        if(M5Cardputer.BtnA.isPressed()) {
            return;
        }

        if(M5Cardputer.Keyboard.isKeyPressed(' ')){
            break;
        }
    }

    
    c.fillRect(25 + 6 * 25, 1 + 3 * 20, 20, 15, TFT_BLACK); 
    c.drawRect(25 + 6 * 25, 1 + 3 * 20, 20, 15, TFT_WHITE); // draw green key white

    c.fillRect(25 + 4 * 25, 1 + 2 * 20, 20, 15, TFT_ORANGE); // draw orange arrow keys
    
    c.fillRect(25 + 3 * 25, 1 + 3 * 20, 20, 15, TFT_ORANGE); // draw green key white
    c.fillRect(25 + 4 * 25, 1 + 3 * 20, 20, 15, TFT_ORANGE); // draw green key white
    c.fillRect(25 + 5 * 25, 1 + 3 * 20, 20, 15, TFT_ORANGE); // draw green key white

    c.fillRect(0, c.height() - 40, c.width(), 50, TFT_BLACK);


    c.drawCenterString("Press [arrow] to", c.width() / 2, c.height() - 40, &fonts::FreeMono9pt7b);
    c.drawCenterString("naviage the FW", c.width() / 2, c.height() - 20, &fonts::FreeMono9pt7b);

    c.pushSprite(0, 0);

    while(true){
        M5Cardputer.update();

        if(M5Cardputer.BtnA.isPressed()) {
            return;
        }

        if(M5Cardputer.Keyboard.isKeyPressed(',') || M5Cardputer.Keyboard.isKeyPressed('.') || M5Cardputer.Keyboard.isKeyPressed('/') || M5Cardputer.Keyboard.isKeyPressed(';')){
            break;
        }
    }


    while (M5Cardputer.Keyboard.isPressed()){
        M5Cardputer.update();
    }

    c.clear();

    c.drawLine(20, 20, c.width() - 20, 20, TFT_WHITE); 
    c.drawLine(c.width() - 20, 20, c.width() - 20, 60, TFT_WHITE); 
    

    c.drawLine(c.width() - 25, 25, c.width() - 120, 25, TFT_WHITE); 
    c.drawLine(c.width() - 25, 25, c.width() - 25, 60, TFT_WHITE); 
    c.drawLine(c.width() - 120, 25, c.width() - 120, 60, TFT_WHITE);

    c.drawRightString("M5", c.width() - 125, 22, &fonts::DejaVu40);

    c.fillCircle(c.width() - 32, 32, 5, TFT_RED);

    String scrollMessage = "Ensure that the tool is turned off before unplugging the USB, press [any key] to continue";
    int messageWidth = c.textWidth(scrollMessage, &fonts::FreeMono9pt7b);

    // c.drawCenterString("Ensure the tool is off", c.width() / 2, c.height() - 60, &fonts::FreeMono9pt7b);
    // c.drawCenterString("Before disconnecting usb", c.width() / 2, c.height() - 40, &fonts::FreeMono9pt7b);
    
    // c.drawCenterString("Press [any] to continue", c.width() / 2, c.height() - 20, &fonts::FreeMono9pt7b);
    // c.drawLine(c.width() - 40, 40, c.width() - 45, 45, TFT_RED);
    // c.drawLine(c.width() - 32, 40, c.width() - 32, 50, TFT_RED);
    // c.drawLine(c.width() - 40, 32, c.width() - 50, 32, TFT_RED);


    c.pushSprite(0, 0);
    // M5Cardputer.Display.setEpdMode(epd_mode_t::epd_fastest);

    unsigned long scrollOffset = millis();
    while(true){
        M5Cardputer.update();

        c.fillRect(0, c.height() - 40, c.width(), c.fontHeight(&fonts::FreeMono9pt7b), TFT_BLACK);
        c.drawString(scrollMessage, c.width() - ((millis() - scrollOffset) / 15 % (messageWidth + c.width())), c.height() - 40, &fonts::FreeMono9pt7b);

        c.drawLine(c.width() - 40, 40, c.width() - 45, 45, millis() % 1000 > 500 ? TFT_RED : TFT_BLACK);
        c.drawLine(c.width() - 32, 40, c.width() - 32, 50, millis() % 1000 > 500 ? TFT_RED : TFT_BLACK);
        c.drawLine(c.width() - 40, 32, c.width() - 50, 32, millis() % 1000 > 500 ? TFT_RED : TFT_BLACK);

        c.waitDisplay();
        c.pushSprite(0, 0);

        if(M5Cardputer.BtnA.isPressed()) {
            return;
        }

        if(M5Cardputer.Keyboard.isPressed()){
            break;
        }
    }

    
    while (M5Cardputer.Keyboard.isPressed()){
        M5Cardputer.update();
    }


    c.clear();

    int topPosX = c.width()/2 - 60;
    int topPosY = 40;
    c.drawRoundRect(topPosX, topPosY, 120, topPosY, 3, TFT_WHITE);
    c.drawLine(topPosX, topPosY + 20, topPosX + 120, topPosY + 20);

    c.drawBezier(topPosX + 90, topPosY+20+10, topPosX + 90, topPosY +20, topPosX + 90 + 10, topPosY + 20, topPosX + 90 + 10, topPosY+20+10); // right shoulder
    c.drawBezier(topPosX + 20, topPosY+20+10, topPosX + 20, topPosY +20, topPosX + 20 + 10, topPosY + 20, topPosX + 20 + 10, topPosY+20+10); // left shoulder

    c.drawLine(topPosX + 40, topPosY + 25, topPosX + 50, topPosY + 25, TFT_WHITE); // SD slot

    c.fillCircle(topPosX + 10, topPosY+22, 3, 0x8016); // IR dot

    c.drawWideLine(topPosX + 70, topPosY + 30, topPosX + 65, topPosY + 30, 5, TFT_WHITE);
    c.drawWideLine(topPosX + 70, topPosY + 30, topPosX + 65, topPosY + 30, 4, TFT_BLACK);
    c.fillCircle(topPosX + 65, topPosY + 30, 2, TFT_WHITE); // on/off switch


    c.fillArc(topPosX + 95, topPosY + 24, 10, 13, 0, 360, TFT_RED);

    c.drawCenterString("Press G0 for the menu", c.width() / 2, c.height() - 20, &fonts::FreeMono9pt7b);

    c.pushSprite(0, 0);

    while (!M5Cardputer.BtnA.isPressed()){
        M5Cardputer.update();
    }

    while (M5Cardputer.BtnA.isPressed()){
        M5Cardputer.update();
    }

    c.clear();
}