#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <stdint.h>
#include <FastLED.h>

#define HIGH8       0xFF    //255
#define HIGH10      0x03FF  //1023
#define HIGH12      0x0FFF  //4095
#define HIGH16      0xFFFF  //65535
#define FACTOR      HIGH8
#define RANGE       HIGH10

enum LED_MODE {
    LED_NONE,
    LED_BREATH,
    LED_FADE,
    LED_JUMP,
    LED_FLASH,
    LED_JUMPBREATH
};

class LedStrip {
    public:
        LedStrip(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, uint8_t pin_w, CRGB const &correction);
        void init();

        void rgbOn();
        void rgbOff();
        void setColor(CRGB color);
        void setColor(CHSV color);
        void setMode(LED_MODE mode);
        void setBrightness(uint8_t value);
        void addBrightness(uint8_t value);
        void subBrightness(uint8_t value);
        void modeFaster();
        void modeSlower();

        void whiteOn();
        void whiteOff();
        void setWhite(uint8_t value);
        void addWhite(uint8_t value);
        void subWhite(uint8_t value);

        void process();
    
    private:
        void setLeds();
        void setWhite();
        void setCorrection();
        void setColor(CRGB color, bool keep);
        void setColor(CHSV color, bool keep);

        bool mRGBOn;
        CRGB mColor;
        LED_MODE mMode;
        uint16_t mModeIndex;
        uint8_t mBrightness;
        unsigned long mNextState;
        unsigned long mStateDuration;
        CHSV mSpecial;

        bool mWhiteOn;
        uint8_t mWhiteBrightness;

        CRGB mCorrection;

        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t w;
        } mPins;
};

#endif //LEDSTRIP_H