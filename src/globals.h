
#ifndef myGlobalsFile 
#define myGlobalsFile 1

#include <Arduino.h>


enum UsbKeyboardState_t
{
    _state_wait_connect = 0,
    _state_mounted,
};

extern UsbKeyboardState_t _USB_PORT_STATUS;
extern bool _is_state_updated;

const int TFT_BACKGROUND_COLOR = 0x528a;// 0X1a1a1a;

#endif