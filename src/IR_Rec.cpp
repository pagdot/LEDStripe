#include "IR_Rec.h"

#include <IRRemoteESP8266.h>
#include <IRutils.h>


IR_Rec::IR_Rec(uint16_t pin) : rec(pin) {
    rec.enableIRIn();  // Start the receiver
    Serial.println("IR module initialized");
}

bool IR_Rec::receive(IR_CMD *cmd) {
    static IR_CMD last;
    if (cmd == nullptr) {
        Serial.println("[ERROR] IR_Rec::recieve: \"cmd\" should not contain a nullpointer");
        return false;
    }
    decode_results data;
    if (rec.decode(&data)) {
        rec.resume();  // Receive the next value
        if (!data.repeat) {
            last = (IR_CMD)data.value;
        }
        *cmd = last;
        return true;
    }
    return false;
}