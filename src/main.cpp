#include <Arduino.h>
#include "IR_Rec.h"
#include "LedStrip.h"
#include <stdio.h>

#include <array>

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).

#define RANGE 256
#define STEP 8

#define PIN_IR D5
#define PIN_W  D6
#define PIN_R  D1
#define PIN_G  D2
#define PIN_B  D0

const CRGB correction(0xffd0e0);

IR_Rec irRec(PIN_IR);
LedStrip leds(PIN_R, PIN_G, PIN_B, PIN_W, correction);

void setup() {
    Serial.begin(9600);
    Serial.write("serial init\n");

    leds.init();
    Serial.write("RGBW init\n");    
}

void loop() {
    IR_CMD cmd;

    static bool enable = false;
    static uint16_t dc = RANGE-1;
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
                leds.setColor(CRGB::OrangeRed);
                break;
            case RGB_R2:
                Serial.println("RGB RED 2");
                leds.setColor(CRGB::Orange);
                break;
            case RGB_R3:
                Serial.println("RGB RED 3");
                leds.setColor(CRGB::LightSalmon);
                break;
            case RGB_R4:
                Serial.println("RGB RED 4");
                leds.setColor(CRGB::Yellow);
                break;
            case RGB_G:
                Serial.println("RGB GREEN");
                leds.setColor(CRGB::Green);
                break;
            case RGB_G1:
                Serial.println("RGB GREEN 1");
                leds.setColor(CRGB::MediumSpringGreen);
                break;
            case RGB_G2:
                Serial.println("RGB GREEN 2");
                leds.setColor(CRGB::MediumTurquoise);
                break;
            case RGB_G3:
                Serial.println("RGB GREEN 3");
                leds.setColor(CRGB::CornflowerBlue);
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
                leds.setColor(CRGB::RoyalBlue);
                Serial.println("RGB BLUE 1");
                break;
            case RGB_B2:
                leds.setColor(CRGB::MidnightBlue);
                Serial.println("RGB BLUE 2");
                break;
            case RGB_B3:
                leds.setColor(CRGB::Indigo);
                Serial.println("RGB BLUE 3");
                break;
            case RGB_B4:
                leds.setColor(CRGB::HotPink);
                Serial.println("RGB BLUE 4");
                break;
            case RGB_W:
                Serial.println("RGB WHITE");
                leds.setColor(CRGB::White);
                break;
            case RGB_W1:
                Serial.println("RGB WHITE 1");
                leds.setColor(CRGB::LightPink);
                break;
            case RGB_W2:
                Serial.println("RGB WHITE 2");
                leds.setColor(CRGB::Orchid);
                break;
            case RGB_W3:
                Serial.println("RGB WHITE 3");
                leds.setColor(CRGB::PaleTurquoise);
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
            default:
                Serial.println("Unknwon CMD");
                break;
        }
    }

    if (Serial.available()) {
        char ch = Serial.read();
        static CRGB rgb(0xffffff);
        static auto col = &(rgb.r);
        switch (ch) {
            case '+':
                (*col) = qadd8(*col, 8);
                break;
            case '-':
                (*col) = qsub8(*col, 8);
                break;
            case 'r':
                col = &(rgb.r);
                break;
            case 'g':
                col = &(rgb.g);
                break;
            case 'b':
                col = &(rgb.b);
                break;
            default:
                break;
        }
        leds.setColor(rgb);
    }
}