#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <stdint.h>
#include <FastLED.h>

enum LED_MODE {
    LED_NONE,
    LED_BREATH,
    LED_FADE,
    LED_JUMP,
    LED_FLASH,
    LED_AUTO
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

        bool mRGBOn;
        CRGB mColor;
        LED_MODE mMode;
        uint16_t mModeIndex;
        uint8_t mBrightness;

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