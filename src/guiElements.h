
#include "M5GFX.h"
#include <stdint.h>

void drawVolumeBar(M5Canvas &canvas, int32_t x, int32_t y, int32_t w, int32_t h, int32_t progress, int32_t max_progress, int border_color, int active_color, int inactive_color);
void drawVolumeBar(M5Canvas &canvas, String label, int32_t x, int32_t y, int32_t w, int32_t h, int32_t progress, int32_t max_progress, int background_color, int border_color, int active_color, int inactive_color);

void drawLabeledBox(M5Canvas &canvas, String label, int32_t x, int32_t y, int32_t w, int32_t h, int background_color, int border_color);
void drawLabeledTextBox(M5Canvas &canvas, String label, String text, int32_t x, int32_t y, int32_t w, int32_t h, int background_color, int border_color);

void drawCrossedBox(M5Canvas &canvas, int32_t x, int32_t y, int32_t w, int32_t h);


void drawKeyboard(M5Canvas &canvas, unsigned int x, unsigned int y);