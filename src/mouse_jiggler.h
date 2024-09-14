void be_a_mouse_jiggler(M5Canvas &canvas, USBHIDMouse &USB_Mouse);


void moveMouse(M5Canvas &canvas, USBHIDMouse &USB_Mouse, int deltaX, int deltaY);
void drawMouse(M5Canvas &canvas, bool isUsbConnected, int drawX, int drawY, bool goLeft, bool goRight, bool goUp, bool goDown);

void disableMouseJiggler();