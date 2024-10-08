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
#include "keyboard_masher.h"
#include "regular_keyboard.h"
#include "regular_mouse.h"

#include "globals.h"

#include "intro.h"

USBHIDKeyboard USB_Keyboard;
USBHIDMouse USB_Mouse;

M5Canvas canvas(&M5Cardputer.Display);

#define PIN_RGB_LED 21
Adafruit_NeoPixel _rgbLed;
int rainbowColor = 0;

const char* mainMenuOptions[] = {"Auto clicker", "Mouse jiggler", "Mash keyboard", "Be a keyboard", "Be a mouse"};
const unsigned int mainMenuSize = sizeof(mainMenuOptions) / sizeof(*mainMenuOptions);
int menuIndex = 0;
int currentAction = 0;

// global thingys ?
UsbKeyboardState_t _USB_PORT_STATUS;
bool _IS_USB_STATE_UPDATED;

// put function declarations here:
void drawMenuInterface();
void be_a_menu();
void prepare_next_application(int &appIndex);

void drawUsbConnectionStatus();

//  esptool.py --chip esp32s3 merge_bin -o combine.bin 0x0 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin

// Invoked when device is mounted
extern "C" void tud_mount_cb(void) {
    _USB_PORT_STATUS = _USB_STATE_CONNECTED;
    _IS_USB_STATE_UPDATED = true;
}

// Invoked when device is unmounted
extern "C" void tud_umount_cb(void) {
    _USB_PORT_STATUS = _USB_STATE_WAITING;
    _IS_USB_STATE_UPDATED = true;
}

void setup() {
    // init globals
    _USB_PORT_STATUS = _USB_STATE_WAITING;
    _IS_USB_STATE_UPDATED = false;

    _rgbLed = Adafruit_NeoPixel(1, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);


    USB_Keyboard.begin();
    USB_Mouse.begin();
    
    // auto cfg = M5.config();
    // M5Cardputer.begin(cfg, true);
    M5Cardputer.begin(true);

    canvas.createSprite(M5Cardputer.Display.width(), M5Cardputer.Display.height());
    canvas.pushSprite(0, 0);
    canvas.setSwapBytes(true);

    M5Cardputer.Speaker.setVolume(50);

    runIntro(canvas);

    USB.begin();

    drawMenuInterface();

    _rgbLed.begin();
    _rgbLed.setBrightness(50);  // Set LED brightness (0-255)
      
    _rgbLed.setPixelColor(0,0);
    _rgbLed.show(); // Reset color to black/off
}

void loop() {
    // put your main code here, to run repeatedly:
    M5Cardputer.update();

    // leds[0] = CRGB::Black; //CHSV(led_ih, 255, 150);
    //  _rgbLed.setPixelColor(0, _rgbLed.ColorHSV(millis()*18, 255, 255));
    //  rainbowColor += 1;
    //  pixels.show();

    if (M5Cardputer.BtnA.isPressed() && currentAction != 0) {
        currentAction = 0;  // Reset back to menu
        menuIndex = 0;

        disableAutoClicker();
        disableMouseJiggler();
        disableKeyboardMasher();
        disableRegularKeyboard();
        disableRegularMouse();

        USB_Mouse.release();
        USB_Keyboard.releaseAll();

        drawMenuInterface();

        // turn off led
        _rgbLed.setPixelColor(0, 0);
        _rgbLed.show();
        return;
    }

    switch (currentAction) {
        case -1:
            runIntro(canvas);
            currentAction = 0;
            break;
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
            be_a_keyboard_masher(canvas, USB_Keyboard);
            break;
        case 4:
            be_a_keyboard(canvas, USB_Keyboard);
            break;
        case 5:
            be_a_mouse(canvas, USB_Mouse);
            break;
        default:
            break;
    }

    canvas.pushSprite(0, 0);
}

void drawMenuInterface() {
    canvas.clear(TFT_BACKGROUND_COLOR);

    String modeStr = mainMenuOptions[menuIndex];
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
        case 3:
            break;
        case 4:
            break;
        default:
            modeStr = "Unknown > " + String(menuIndex);
            break;
    }

    canvas.setTextColor(0xffffffff);
    canvas.setFont(&fonts::FreeMono12pt7b);
    canvas.drawCenterString(modeStr, M5Cardputer.Display.width() / 2, 10);

    canvas.fillTriangle(25, 5, 25, 31, 12, 18, menuIndex == 0 ? TFT_RED : TFT_GREEN);
    canvas.fillTriangle(canvas.width() - 25, 5, canvas.width() - 25, 31, canvas.width() - 12, 18, menuIndex == mainMenuSize -1 ? TFT_RED : TFT_GREEN);


    // canvas.setTextColor(TFT_WHITE);
    // canvas.setFont(&fonts::DejaVu9);
    // canvas.drawRightString("Press G0 to come back", canvas.width(), canvas.height() - canvas.fontHeight());
    // canvas.drawString("Press P or [space] to toggle state", 0, canvas.height() - (canvas.fontHeight()*2));
}

void be_a_menu() {
    drawUsbConnectionStatus();

    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isKeyPressed(';') || M5Cardputer.Keyboard.isKeyPressed(',')) {  // up
            menuIndex--;
        } else if (M5Cardputer.Keyboard.isKeyPressed('.') || M5Cardputer.Keyboard.isKeyPressed('/')) {  // down
            menuIndex++;
        } else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) || M5Cardputer.Keyboard.isKeyPressed(' ')) {
            currentAction = menuIndex + 1;
            prepare_next_application(currentAction);
            canvas.clear(TFT_BACKGROUND_COLOR);
        }

        menuIndex = constrain(menuIndex, 0, mainMenuSize - 1);

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

    // Reset fonts
    canvas.setFont(&fonts::FreeMono12pt7b);

    if(appIndex == 4 || appIndex == 5){
        if(_USB_PORT_STATUS == _USB_STATE_CONNECTED){
            _rgbLed.setPixelColor(0, _rgbLed.Color(0,255,0));
        } else {
            _rgbLed.setPixelColor(0, _rgbLed.Color(255,0,0));
        } 
        _rgbLed.show();
    } else {
  
        _rgbLed.setPixelColor(0, _rgbLed.Color(255,0,0));
        _rgbLed.show();
    }

}


void drawUsbConnectionStatus(){
    // canvas.drawBezier(100, 50, 100, 60, 110, 60, 110, 50);
    // canvas.drawWedgeLine(100, 50, 120, 70, 5, 5, TFT_RED);

    int cable_x = canvas.width() / 2 - 35;
    int cable_y = 50;
    bool isConnected = _USB_PORT_STATUS == _USB_STATE_CONNECTED;

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