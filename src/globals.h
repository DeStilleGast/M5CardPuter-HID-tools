
#include <Arduino.h>

enum UsbKeyboardState_t
{
    _state_wait_connect = 0,
    _state_mounted,
};

static UsbKeyboardState_t _USB_PORT_STATUS = _state_wait_connect;
static bool _is_state_updated = false;

const int TFT_BACKGROUND_COLOR = 0x528a;// 0X1a1a1a;

