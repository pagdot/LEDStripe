#include <Arduino.h>
#include "IR_Rec.h"
#include "LedStrip.h"
#include <stdio.h>

#include <array>

#define PIN_IR D5
#define PIN_W  D6
#define PIN_R  D1
#define PIN_G  D2
#define PIN_B  D0

const CRGB correction(0xFFB0D0);

IR_Rec irRec(PIN_IR);
LedStrip leds(PIN_R, PIN_G, PIN_B, PIN_W, correction);

void setup() {
    Serial.begin(9600);
    Serial.write("serial init\n");

    leds.init();
    Serial.write("RGBW init\n");    

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    IR_CMD cmd;

    if (irRec.receive(&cmd)) {
        
        switch(cmd) {
            case RGB_ON:
                Serial.println("RGB ON");
                leds.rgbOn();
                break;
            case RGB_OFF:
                Serial.println("RGB OFF");
                leds.rgbOff();
                break;
            case RGB_UP:
                Serial.println("RGB UP");
                leds.addBrightness(8);
                break;
            case RGB_DOWN:
                Serial.println("RGB DOWN");
                leds.subBrightness(8);
                break;
            case RGB_R:
                Serial.println("RGB RED");
                leds.setColor(CRGB::Red);
                break;
            case RGB_R1:
                Serial.println("RGB RED 1");
                leds.setColor(0xFF1000);
                break;
            case RGB_R2:
                Serial.println("RGB RED 2");
                leds.setColor(0xFF3000);
                break;
            case RGB_R3:
                Serial.println("RGB RED 3");
                leds.setColor(0xFF5000);
                break;
            case RGB_R4:
                Serial.println("RGB RED 4");
                leds.setColor(0xFF7000);
                break;
            case RGB_G:
                Serial.println("RGB GREEN");
                leds.setColor(CRGB::Green);
                break;
            case RGB_G1:
                Serial.println("RGB GREEN 1");
                leds.setColor(CRGB::Turquoise);
                break;
            case RGB_G2:
                Serial.println("RGB GREEN 2");
                leds.setColor(CRGB::DarkTurquoise);
                break;
            case RGB_G3:
                Serial.println("RGB GREEN 3");
                leds.setColor(CRGB::SeaGreen);
                break;
            case RGB_G4:
                Serial.println("RGB GREEN 4");
                leds.setColor(CRGB::DarkCyan);
                break;
            case RGB_B:
                leds.setColor(CRGB::Blue);
                Serial.println("RGB BLUE");
                break;
            case RGB_B1:
                leds.setColor(CRGB::SteelBlue);
                Serial.println("RGB BLUE 1");
                break;
            case RGB_B2:
                leds.setColor(CRGB::MidnightBlue);
                Serial.println("RGB BLUE 2");
                break;
            case RGB_B3:
                leds.setColor(CRGB::Purple);
                Serial.println("RGB BLUE 3");
                break;
            case RGB_B4:
                leds.setColor(CRGB::DeepPink);
                Serial.println("RGB BLUE 4");
                break;
            case RGB_W:
                Serial.println("RGB WHITE");
                leds.setColor(CRGB::White);
                break;
            case RGB_W1:
                Serial.println("RGB WHITE 1");
                leds.setColor(CRGB::Fuchsia);
                break;
            case RGB_W2:
                Serial.println("RGB WHITE 2");
                leds.setColor(CRGB::MediumVioletRed);
                break;
            case RGB_W3:
                Serial.println("RGB WHITE 3");
                leds.setColor(CRGB::RoyalBlue);
                break;
            case RGB_W4:
                Serial.println("RGB WHITE 4");
                leds.setColor(CRGB::MediumTurquoise);
                break;
            case W_ON:
                Serial.println("White ON");
                leds.whiteOn();
                break;
            case W_OFF:
                Serial.println("White OFF");
                leds.whiteOff();
                break;
            case W_UP:
                Serial.println("White UP");
                leds.addWhite(8);
                break;
            case W_DOWN:
                Serial.println("White DOWN");
                leds.subWhite(8);
                break;
            case W_25:
                Serial.println("White 25\%");
                leds.setWhite(64);
                break;
            case W_50:
                Serial.println("White 50\%");
                leds.setWhite(128);
                break;
            case W_75:
                Serial.println("White 75\%");
                leds.setWhite(192);
                break;
            case W_100:
                Serial.println("White 100\%");
                leds.setWhite(255);
                break;
            case JUMP3:
                Serial.println("Mode Jump3");
                leds.setMode(LED_JUMP);
                break;
            case FADE3:
                Serial.println("Mode Fade3");
                leds.setMode(LED_FADE);
                break;
            case FLASH:
                Serial.println("Mode Flash");
                leds.setMode(LED_FLASH);
                break;
            case FADE7:
                Serial.println("Mode Breath");
                leds.setMode(LED_BREATH);
                break;
            case AUTO:
                Serial.println("Mode Jump-Breath");
                leds.setMode(LED_JUMPBREATH);
            case QUICK:
                Serial.println("Faster");
                leds.modeFaster();
                break;
            case SLOW:
                Serial.println("Slower");
                leds.modeSlower();
                break;
            default:
                if ((cmd & 0xff0000) == 0xff0000) {
                    Serial.printf("Unknwon CMD %x\n", cmd);
                }
                break;
        }
    }
    
    leds.process();
}