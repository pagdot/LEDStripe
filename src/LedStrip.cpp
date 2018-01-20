#include <LedStrip.h>

#define LOW8  0x00
#define HIGH8 0xFF
#define FACTOR HIGH8
#define RANGE 256

LedStrip::LedStrip(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, uint8_t pin_w, CRGB const &correction) : 
        mMode(LED_NONE), mColor(0), mRGBOn(false), mWhiteOn(false), 
        mBrightness(HIGH8), mWhiteBrightness(HIGH8), mCorrection(correction) {
    mPins.r = pin_r;
    mPins.g = pin_g;
    mPins.b = pin_b;
    mPins.w = pin_w;
}

void LedStrip::init() {
    
    pinMode(mPins.r, OUTPUT);
    pinMode(mPins.g, OUTPUT);
    pinMode(mPins.b, OUTPUT);
    pinMode(mPins.w, OUTPUT);
    analogWriteRange(RANGE);

    setLeds();
    setWhite();
}

void LedStrip::rgbOn() {
    mRGBOn = true;
    setLeds();
}

void LedStrip::rgbOff() {
    mRGBOn = false;
    setLeds();
}

void LedStrip::setColor(CRGB color) {
    mColor = color;
    mMode = LED_NONE;
    setLeds();
}

void LedStrip::setColor(CHSV color) {
    hsv2rgb_rainbow(color, mColor);
    mMode = LED_NONE;
    setLeds();
}

void LedStrip::setMode(LED_MODE mode) {
    mMode = mode;
    mModeIndex = 0;
    setLeds();
}

void LedStrip::setBrightness(uint8_t value) {
    mBrightness = value;
    setLeds();
}

void LedStrip::addBrightness(uint8_t value) {
    mBrightness = qadd8(mBrightness, value);
    setLeds();
}

void LedStrip::subBrightness(uint8_t value) {
    mBrightness = qsub8(mBrightness, value);
    setLeds();
}

void LedStrip::whiteOn() {
    mWhiteOn = true;
    setWhite();
}

void LedStrip::whiteOff() {
    mWhiteOn = false;
    setWhite();
}

void LedStrip::setWhite(uint8_t value) {
    mWhiteBrightness = value;
    setWhite();
}

void LedStrip::addWhite(uint8_t value) {
    mWhiteBrightness = qadd8(mWhiteBrightness, value);
    setWhite();
}

void LedStrip::subWhite(uint8_t value) {
    mWhiteBrightness = qsub8(mWhiteBrightness, value);
    setWhite();
}

void LedStrip::process() {
    static unsigned long next = 0;
    unsigned long now = millis();
    
    if ((next < now) || (mMode == LED_NONE) || (!mRGBOn)) {
        return;
    } 
    
    switch (mMode) {
        case LED_BREATH:
            setBrightness(quadwave8(mModeIndex));
            break;
        case LED_FADE:
            {
                CHSV hsv;
                hsv.hue = mModeIndex;
                hsv.sat = HIGH8;
                hsv.val = HIGH8;
                setColor(hsv);
                next = now + 500;
            }
            break;
        case LED_JUMP:
            if (mModeIndex%16 == 0) {
                CHSV hsv;
                hsv.hue = mModeIndex;
                hsv.sat = HIGH8;
                hsv.val = HIGH8;
                setColor(hsv);
                next = now + 500;
            }
            break;
        case LED_FLASH:
            if (mModeIndex%16 == 0) {
                CHSV hsv;
                hsv.hue = mModeIndex;
                hsv.sat = HIGH8;
                hsv.val = HIGH8;
                setColor(hsv);
            } else if ((mModeIndex+8)%16 == 0) {
                CRGB rgb(0);
                setColor(rgb);
            }

            break;
        default:
            break;
    }
    setLeds();
}

void LedStrip::setLeds() {
    uint8_t max = mColor.r;
    if (mColor.g > max) max = mColor.g;
    if (mColor.b > max) max = mColor.b;
    
    if (mRGBOn && (max > 0)) {
        uint16_t factor = (((uint16_t) mBrightness) * FACTOR) / max;
        Serial.printf("Color: %0.2x%0.2x%0.2x Factor: %d\n", mColor.r, mColor.g, mColor.b, factor);
        analogWrite(mPins.r, mColor.r * factor/FACTOR);
        analogWrite(mPins.g, mColor.g * factor/FACTOR);
        analogWrite(mPins.b, mColor.b * factor/FACTOR);
    } else {
        Serial.printf("RGB-LEDs OFF\n");
        analogWrite(mPins.r, LOW8);
        analogWrite(mPins.g, LOW8);
        analogWrite(mPins.b, LOW8);
    }
}

void LedStrip::setWhite() {
    if (mWhiteOn) {
        Serial.printf("White: Brightness: %d\n", mWhiteBrightness);
        analogWrite(mPins.w, mWhiteBrightness);
    } else {
        Serial.printf("White: OFF\n");
        analogWrite(mPins.w, LOW8);
    }
}


void LedStrip::setCorrection() {
    mCorrection.b = scale8(mColor.r, mCorrection.r);
    mCorrection.g = scale8(mColor.g, mCorrection.g);
    mCorrection.b = scale8(mColor.b, mCorrection.b);
}