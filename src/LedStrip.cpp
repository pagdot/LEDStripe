#include <LedStrip.h>

static inline uint16_t div8to10(uint16_t const base, uint16_t const div, bool zero = true);

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
    setColor(color, false);
}

void LedStrip::setColor(CHSV color) {
    setColor(color, false);
}

void LedStrip::setColor(CRGB color, bool keep) {
    if (!keep) mMode = LED_NONE;
    mColor = color;
    setLeds();
}

void LedStrip::setColor(CHSV color, bool keep) {
    if (!keep) mMode = LED_NONE;
    hsv2rgb_rainbow(color, mColor);
    setLeds();
}

void LedStrip::setMode(LED_MODE mode) {
    mMode = mode;
    mModeIndex = 0;

    switch(mode) {
        
    case LED_NONE:
        break;
    case LED_BREATH:
        mStateDuration = 1;
        break;
    case LED_FADE:
        mStateDuration = 200;
        break;
    case LED_JUMP:
        mStateDuration = 60;
        break;
    case LED_FLASH:
        mStateDuration = 50;
        break;
    case LED_JUMPBREATH:
        mStateDuration = 1;
        mSpecial.hue = 0;
        mSpecial.s = HIGH8;
        mSpecial.v = HIGH8;
        break;
    }
    mNextState = millis();

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
    unsigned long now = millis();
    
    if ((mNextState > now) || (mMode == LED_NONE) || (!mRGBOn)) {
        return;
    } 
    Serial.printf("Mode\n");
    
    switch (mMode) {
        case LED_JUMPBREATH:
            {
                if (mModeIndex%HIGH8 == 0) {
                    mSpecial.hue += 32;
                    setColor(mSpecial, true);
                }
                uint8_t tmp = quadwave8(mModeIndex);
                tmp = tmp == 0 ? 1 : tmp;
                setBrightness(tmp);
                break;
            }
        case LED_BREATH:
            {
                uint8_t tmp = quadwave8(mModeIndex);
                tmp = tmp == 0 ? 1 : tmp;
                setBrightness(tmp);
                break;
            }
        case LED_FADE:
            {
                CHSV hsv;
                hsv.hue = mModeIndex % HIGH8;
                hsv.sat = HIGH8;
                hsv.val = HIGH8;
                setColor(hsv, true);
                //if ((mColor.r == 0xab) && (mColor.g == 0xaa) && (mColor.b == 0x00)) {
                //    Serial.printf("Index: %u\nStopping\n", mModeIndex % HIGH8);
                //    delay(4000);
                //    Serial.printf("continue\n");
                //}
            }
            break;
        case LED_JUMP:
            if (mModeIndex%16 == 0) {
                CHSV hsv;
                hsv.hue = mModeIndex;
                hsv.sat = HIGH8;
                hsv.val = HIGH8;
                setColor(hsv, true);
            }
            break;
        case LED_FLASH:
            if (mModeIndex%16 == 0) {
                CHSV hsv;
                hsv.hue = mModeIndex;
                hsv.sat = HIGH8;
                hsv.val = HIGH8;
                setColor(hsv, true);
            } else if ((mModeIndex+8)%16 == 0) {
                CRGB rgb(0);
                setColor(rgb, true);
            }
            break;
        default:
            break;
    }
    mNextState = now + mStateDuration;
    mModeIndex++;
}

void LedStrip::setLeds() {
    uint8_t max = mColor.r;
    if (mColor.g > max) max = mColor.g;
    if (mColor.b > max) max = mColor.b;
    if (mMode != LED_NONE) max = HIGH8;
    
    if (mRGBOn && (max > 0) && (mBrightness > 0)) {
        uint16_t factor = (((uint16_t) mBrightness) * FACTOR) / max;
        Serial.printf("Color: %0.2x%0.2x%0.2x Factor: %d\n", mColor.r, mColor.g, mColor.b, factor);
        analogWrite(mPins.r, div8to10(mColor.r * factor, FACTOR, mColor.r == 0));
        analogWrite(mPins.g, div8to10(mColor.g * factor, FACTOR, mColor.g == 0));
        analogWrite(mPins.b, div8to10(mColor.b * factor, FACTOR, mColor.b == 0));
        Serial.print("\n");
    } else {
        Serial.printf("RGB-LEDs OFF\n");
        analogWrite(mPins.r, LOW);
        analogWrite(mPins.g, LOW);
        analogWrite(mPins.b, LOW);
    }
}

void LedStrip::setWhite() {
    if (mWhiteOn) {
        Serial.printf("White: Brightness: %d\n", mWhiteBrightness);
        analogWrite(mPins.w, mWhiteBrightness << 2);
    } else {
        Serial.printf("White: OFF\n");
        analogWrite(mPins.w, LOW);
    }
}

void LedStrip::setCorrection() {
    mCorrection.b = scale8(mColor.r, mCorrection.r);
    mCorrection.g = scale8(mColor.g, mCorrection.g);
    mCorrection.b = scale8(mColor.b, mCorrection.b);
}

void LedStrip::modeFaster(){
    mStateDuration /= 2;
    if (mStateDuration == 0) mStateDuration = 1;
}

void LedStrip::modeSlower(){
    mStateDuration *= 2;
}

static inline uint16_t div8to10(uint16_t const base, uint16_t const div, bool zero) {
    if (base == 0) return 0;
    if (div  == 0) return HIGH16;
    uint16_t result = (base << 2) / div;
    return  zero || (result != 0) ? result : 1; 
}