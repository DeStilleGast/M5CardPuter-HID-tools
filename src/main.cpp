#include <Arduino.h>

#include "M5Cardputer.h"
#include "M5GFX.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include "guiElements.h"
// #include "images.h"

#include <Adafruit_NeoPixel.h>

#include "auto_clicker.h"
#include "mouse_jiggler.h"
#include "globals.h"

USBHIDKeyboard USB_Keyboard;
USBHIDMouse USB_Mouse;

M5Canvas canvas(&M5Cardputer.Display);

#define PIN_RGB_LED 21
Adafruit_NeoPixel _rgbLed;
int rainbowColor = 0;

const char* menu_options[] = {"Auto clicker", "Mouse jiggler", "Be a keyboard"};
int menuIndex = 0;
int currentAction = 0;

// global thingys ?
UsbKeyboardState_t _USB_PORT_STATUS;
bool _is_state_updated;

// put function declarations here:
void drawMenuInterface();
void be_a_menu();
void prepare_next_application(int &appIndex);

void be_a_keyboard();
void drawUsbConnectionStatus();

//  esptool.py --chip esp32s3 merge_bin -o combine.bin 0x0 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin

// Invoked when device is mounted
extern "C" void tud_mount_cb(void) {
    _USB_PORT_STATUS = _state_mounted;
    _is_state_updated = true;
}

// Invoked when device is unmounted
extern "C" void tud_umount_cb(void) {
    _USB_PORT_STATUS = _state_wait_connect;
    _is_state_updated = true;
}

void setup() {
    // init globals
    _USB_PORT_STATUS = _state_wait_connect;
    _is_state_updated = false;

    _rgbLed = Adafruit_NeoPixel(1, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);




    USB_Keyboard.begin();
    USB_Mouse.begin();
    USB.begin();

    // USB.onEvent(usbEventCallback);

    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);

    canvas.createSprite(M5Cardputer.Display.width(), M5Cardputer.Display.height());
    canvas.pushSprite(0, 0);
    canvas.setSwapBytes(true);

    M5Cardputer.Speaker.setVolume(50);

    drawMenuInterface();

    _rgbLed.begin();
    _rgbLed.setBrightness(50);  // Set LED brightness (0-255)
}

void loop() {
    // put your main code here, to run repeatedly:
    M5Cardputer.update();

    // leds[0] = CRGB::Black; //CHSV(led_ih, 255, 150);
    //  _rgbLed.setPixelColor(0, _rgbLed.ColorHSV(millis()*18, 255, 255));
    //  rainbowColor += 1;
    //  pixels.show();

    if (M5Cardputer.BtnA.isPressed()) {
        currentAction = 0;  // Reset back to menu
        menuIndex = 0;

        disableAutoClicker();

        USB_Mouse.release();

        drawMenuInterface();

        _rgbLed.setPixelColor(0, 0);
        _rgbLed.show();
        return;
    }

    switch (currentAction) {
        case 0:
            be_a_menu();
            break;
        case 1:
            be_a_auto_clicker(canvas, USB_Mouse);
            break;
        case 2:
            be_a_mouse_jiggler(canvas, USB_Mouse);
            break;
        case 3:
            be_a_keyboard();
        default:
            break;
    }

    // canvas.fillTriangle(canvas.width(), 0, canvas.width() - 50, 0, canvas.width(), 50, _USB_PORT_STATUS == _state_mounted ? TFT_GREEN : TFT_RED);

    canvas.pushSprite(0, 0);
}

void drawMenuInterface() {
    canvas.clear(TFT_BACKGROUND_COLOR);

    String modeStr = menu_options[menuIndex];
    ;

    switch (menuIndex) {
        case 0:
            // canvas.pushImage(canvas.width() / 2 - 32, canvas.height() / 2 - 32,  64, 64, image_data_cursor, TFT_BLACK);
            break;
        case 1:
            break;
        case 2:
            // canvas.pushImage(canvas.width() / 2 - 32, canvas.height() / 2 - 32,  64, 64, image_data_keyboard_big, TFT_BLACK);
            break;
        default:
            modeStr = "??? > " + currentAction;
            break;
    }

    canvas.setTextColor(0xffffffff);
    canvas.setFont(&fonts::FreeMono12pt7b);
    canvas.drawCenterString(modeStr, M5Cardputer.Display.width() / 2, 10);

}

void be_a_menu() {
    drawUsbConnectionStatus();

    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isKeyPressed(';') || M5Cardputer.Keyboard.isKeyPressed(',')) {  // up
            if (menuIndex > 0)
                menuIndex--;
        } else if (M5Cardputer.Keyboard.isKeyPressed('.') || M5Cardputer.Keyboard.isKeyPressed('/')) {  // down
            if (menuIndex < 2)
                menuIndex++;
        } else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            currentAction = menuIndex + 1;
            prepare_next_application(currentAction);
            canvas.clear(TFT_BACKGROUND_COLOR);
        }

        // if (menuIndex >= 2)
        //   menuIndex = 2;
        // if (menuIndex <= 0)
        //   menuIndex = 0;

        menuIndex = constrain(menuIndex, 0, 2);

        if (M5Cardputer.Keyboard.isPressed()) {
            if (currentAction == 0)
                drawMenuInterface();
            // M5Cardputer.Speaker.tone(7000, 20);
        } else {
            // M5Cardputer.Speaker.tone(5000, 20);
        }
    }
}

void prepare_next_application(int &appIndex){
  if(appIndex == 1 || appIndex == 2){
    _rgbLed.setPixelColor(0, _rgbLed.Color(255,0,0));
    _rgbLed.show();
  }

}


void be_a_keyboard() {
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            // for (auto i : status.word) {
            //     Keyboard.press(i);
            // }
            KeyReport report = {0};
            report.modifiers = status.modifiers;
            uint8_t index = 0;
            for (auto i : status.hid_keys) {
                report.keys[index] = i;
                index++;
                if (index > 5) {
                    index = 5;
                }
            }
            USB_Keyboard.sendReport(&report);
            USB_Keyboard.releaseAll();

            // only text for display
            String keyStr = "";
            for (auto i : status.word) {
                if (keyStr != "") {
                    keyStr = keyStr + "+" + i;
                } else {
                    keyStr += i;
                }
            }

            if (keyStr.length() > 0) {
                canvas.drawString(keyStr, canvas.width() / 2, canvas.height() / 2);
            }
        } else {
            canvas.drawString("USB Keyboard", canvas.width() / 2, canvas.height() / 2);
        }
    }
}

// static void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
//   if(event_base == ARDUINO_USB_EVENTS){
//     arduino_usb_event_data_t * data = (arduino_usb_event_data_t*)event_data;
//     switch (event_id){
//       case ARDUINO_USB_STARTED_EVENT:
//         _USB_PORT_STATUS = _state_mounted;
//         _is_state_updated = true;
//         break;
//       case ARDUINO_USB_STOPPED_EVENT:
//         _USB_PORT_STATUS = _state_wait_connect;
//         _is_state_updated = true;
//         break;
//       case ARDUINO_USB_SUSPEND_EVENT:
//         _USB_PORT_STATUS = _state_wait_connect;
//         _is_state_updated = true;
//         break;
//       case ARDUINO_USB_RESUME_EVENT:
//         _USB_PORT_STATUS = _state_mounted;
//         _is_state_updated = true;
//         break;
//       default:
//         break;
//     }
//   }
// }


void drawUsbConnectionStatus(){
    // canvas.drawBezier(100, 50, 100, 60, 110, 60, 110, 50);
    // canvas.drawWedgeLine(100, 50, 120, 70, 5, 5, TFT_RED);

    int cable_x = canvas.width() / 2 - 35;
    int cable_y = 50;
    bool isConnected = _USB_PORT_STATUS == _state_mounted;

    canvas.fillRect(cable_x, cable_y - 5, 120, 40, TFT_BACKGROUND_COLOR);

    canvas.drawWideLine(cable_x, cable_y, cable_x + 20, cable_y, 3, TFT_WHITE);
    canvas.drawWideLine(cable_x + 20, cable_y, cable_x + 10, cable_y + 15, 3, TFT_WHITE);
    canvas.drawWideLine(cable_x + 10, cable_y + 15, cable_x + 30, cable_y + 15, 3, TFT_WHITE);
    
    canvas.fillRect(cable_x + 26, cable_y + 8, 7, 16);

    
    if(!isConnected){
      canvas.fillRect(cable_x + 32, cable_y + 12, 10, 9, TFT_LIGHTGREY);
    }
    
    cable_x += isConnected ? 40 : 55;
    cable_y += 15;

    canvas.fillRect(cable_x - 7, cable_y - 7, 7, 16, TFT_WHITE);
    canvas.drawWideLine(cable_x, cable_y, cable_x + 20, cable_y, 3, TFT_WHITE);
    canvas.drawWideLine(cable_x + 20, cable_y, cable_x + 10, cable_y + 15, 3, TFT_WHITE);
    canvas.drawWideLine(cable_x + 10, cable_y + 15, cable_x + 30, cable_y + 15, 3, TFT_WHITE);

    if(!isConnected){
        bool swapColors = millis() % 1000 > 500;
        canvas.setTextColor(swapColors ? TFT_YELLOW : TFT_BLUE);

        cable_x -= 10;
        cable_y -= 15;
        
        canvas.fillCircle(cable_x + 44, cable_y + 5 , 10, swapColors ? TFT_BLUE : TFT_YELLOW);
        canvas.drawString("!", cable_x + 40, cable_y - 5, &fonts::DejaVu24);

        // cable_x -= 10;
        // cable_y -= 15;

        // canvas.fillCircle(cable_x + 48, cable_y + 12 , 20, TFT_BLUE);
        // canvas.drawString("!", cable_x + 40, cable_y - 5, &fonts::DejaVu40);
    }
}