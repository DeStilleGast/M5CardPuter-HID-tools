
#ifndef myGlobalsFile 
#define myGlobalsFile 1

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "M5Cardputer.h"


const int TFT_BACKGROUND_COLOR = 0x0; //0x528a;

enum UsbKeyboardState_t
{
    _USB_STATE_WAITING = 0,
    _USB_STATE_CONNECTED,
};

extern UsbKeyboardState_t _USB_PORT_STATUS;
extern bool _IS_USB_STATE_UPDATED;

extern Adafruit_NeoPixel _rgbLed;


#endif