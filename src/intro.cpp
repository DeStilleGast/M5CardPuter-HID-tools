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


    c.clear();

    int topPosX = c.width()/2 - 60;
    int topPosY = 40;
    c.drawRoundRect(topPosX, topPosY, 120, topPosY, 3, TFT_WHITE);
    c.drawLine(topPosX, topPosY + 20, topPosX + 120, topPosY + 20);

    c.drawBezier(topPosX + 90, topPosY+20, topPosX + 90, topPosY +20 + 10, topPosX + 90 + 10, topPosY + 20 + 10, topPosX + 90 + 10, topPosY+20); // right shoulder
    c.drawBezier(topPosX + 20, topPosY+20, topPosX + 20, topPosY +20 + 10, topPosX + 20 + 10, topPosY + 20 + 10, topPosX + 20 + 10, topPosY+20); // left shoulder

    c.drawLine(topPosX + 40, topPosY + 25, topPosX + 50, topPosY + 25, TFT_WHITE); // SD slot

    c.fillCircle(topPosX + 10, topPosY+22, 3, TFT_WHITE); // IR dot

    c.fillArc(topPosX + 95, topPosY + 22, 10, 13, 0, 360, TFT_RED);

    c.drawCenterString("Press G0 to go back", c.width() / 2, c.height() - 20, &fonts::FreeMono9pt7b);

    c.pushSprite(0, 0);

    while (!M5Cardputer.BtnA.isPressed()){
        M5Cardputer.update();
    }
    
    
    while (M5Cardputer.BtnA.isPressed()){
        M5Cardputer.update();
    }

    c.clear();
}