
#include "M5GFX.h"


void drawVolumeBar(M5Canvas &canvas, int32_t x, int32_t y, int32_t w, int32_t h, int32_t progress, int32_t max_progress, int border_color, int active_color, int inactive_color){

    canvas.setColor(border_color);
    canvas.drawRect(x, y, w, h);
    int segmentCount = floor(w / 10) - 1;

    int seg =  map(progress, 0, max_progress, 0, segmentCount);

    for(int i = 0; i < segmentCount; i++) {
        int segOffset = i * 10;

         if(i < seg) {
            canvas.setColor(active_color);
        } else {
            canvas.setColor(inactive_color);
        }

        canvas.fillRect(
                (x - 5) + segOffset + 10,
                y + 2, 
                8,
                h - 4
            );
    }
    // canvas.fillRect(x, y, w, h, TFT_GREEN);
   
    // sprite.fillRect(55,110,220,8,back);
    // sprite.fillRect(60,75,220,8,back);

}


void drawVolumeBar(M5Canvas &canvas, String label, int32_t x, int32_t y, int32_t w, int32_t h, int32_t progress, int32_t max_progress, int background_color, int border_color, int active_color, int inactive_color){
    canvas.setTextColor(TFT_WHITE);
    canvas.fillRect(x, y, canvas.textWidth(label) + 20, canvas.fontHeight(), background_color);
    canvas.drawString(label, x, y);

    drawVolumeBar(canvas, x, y + canvas.fontHeight(), w, h, progress, max_progress, border_color, active_color, inactive_color);
}


void drawCrossedBox(M5Canvas &canvas, int32_t x, int32_t y, int32_t w, int32_t h) {
    canvas.drawWideLine(x, y, x + w, y, 5, TFT_RED);
    canvas.drawWideLine(x, y, x, y + h, 5, TFT_RED);

    canvas.drawWideLine(x + w, y, x + w, y + h, 5, TFT_RED);
    canvas.drawWideLine(x, y + h, x + w, y + h, 5, TFT_RED);

    canvas.drawWideLine(x, y, x + w, y + h, 5, TFT_RED);
    canvas.drawWideLine(x + w, y, x, y + h, 5, TFT_RED);
}


void drawMouseIcon(M5Canvas &canvas, int32_t x, int32_t y, int TFT_BACKGROUND_COLOR){
    // clear mouse
    canvas.fillRect(x - 35, 9, 71, 112, TFT_BACKGROUND_COLOR);

    // mouse
    canvas.drawWedgeLine(x, 50, x, canvas.height() - 50, 30, 30, TFT_WHITE);
    canvas.fillRect(x - 25, 50, 51, 5, TFT_BLACK);
    canvas.fillRect(x - 1, 25, 3, 20, TFT_BLACK);
}