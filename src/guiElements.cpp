
#include "M5GFX.h"
#include "M5Cardputer.h"

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


void drawLabeledBox(M5Canvas &canvas, String label, int32_t x, int32_t y, int32_t w, int32_t h, int background_color, int border_color){
    canvas.setTextColor(TFT_WHITE);
    //canvas.fillRect(x, y, canvas.textWidth(label) + 20, canvas.fontHeight(), background_color);
    // canvas.drawString(label, x, y);
    canvas.fillRect(x, y + canvas.fontHeight(), w, h, background_color);
    canvas.drawCenterString(label, x + w / 2, y );

    canvas.drawRect(x, y + canvas.fontHeight(), w, h, border_color);
}


void drawLabeledTextBox(M5Canvas &canvas, String label, String text, int32_t x, int32_t y, int32_t w, int32_t h, int background_color, int border_color){
    drawLabeledBox(canvas, label, x, y, w, h, background_color, border_color);

    canvas.setClipRect(x, y + canvas.fontHeight(), w, h);
    canvas.drawCenterString(text, 1 + x + (w/2), 1 + y + canvas.fontHeight());
    canvas.clearClipRect();
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

void drawKeyboard(M5Canvas &canvas, unsigned int x, unsigned int y, int colorNotPushed, int colorPushed) {
    canvas.drawRect(x, y, 146, 46, TFT_WHITE); // Outer rectangle
    // canvas.drawRect(12, 12, 104, 42, TFT_WHITE); // Inner rectangle
    
    // Draw keys
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 14; j++) {
            KeyValue_t key = M5Cardputer.Keyboard.getKeyValue(Point2D_t{ j, i });
            


            canvas.drawRect(x + 4 + j * 10, y + 4 + i * 10, 8, 8, M5Cardputer.Keyboard.isKeyPressed(key.value_first) || (M5Cardputer.Keyboard.isKeyPressed(key.value_second) & M5Cardputer.Keyboard.keysState().shift) ? colorPushed :colorNotPushed);
        }
    }
}