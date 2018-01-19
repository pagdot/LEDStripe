#include <Arduino.h>
#include "IR_Rec.h"

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
uint16_t RECV_PIN = 14;

#define RANGE 256
#define STEP 8

IR_Rec irRec(RECV_PIN);

void setup() {
    Serial.begin(9600);
    Serial.write("serial init\n");

    pinMode(LED_BUILTIN, OUTPUT);
    analogWriteRange(RANGE);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.write("LED init\n");    
}

void loop() {
    IR_CMD cmd;
    static bool enable = false;
    static uint16_t dc = RANGE-1;
    if (irRec.receive(&cmd)) {
        switch(cmd) {
            case RGB_ON:
                enable = true;
                break;
            case RGB_OFF:
                enable = false;
                analogWrite(LED_BUILTIN,0);
                digitalWrite(LED_BUILTIN, HIGH);
                break;
            case RGB_UP:
                if (dc < RANGE-STEP) dc += STEP;
                break;
            case RGB_DOWN:
                if (dc >= STEP) dc -= STEP;
                break;
            default:
                Serial.println("Unknwon CMD");
                break;
        }
        if (enable) {
            analogWrite(LED_BUILTIN, RANGE-dc);
            Serial.printf("Enabled, DC: %u\n", dc);
        } else {
            Serial.printf("Disabled, DC: %u\n", dc);
            
        }
    }
}