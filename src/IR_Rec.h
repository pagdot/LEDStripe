#ifndef IR_REC_H
#define IR_REC_H

#include <stdint.h>
#include <IRrecv.h>

enum IR_CMD {
    RGB_UP = 0xFF3AC5,
    RGB_DOWN = 0xFFBA45,
    RGB_OFF = 0xFF827D,
    RGB_ON = 0xFF02FD,

    RGB_R  = 0xFF1AE5,
    RGB_R1 = 0xFF2AD5,
    RGB_R2 = 0xFF0AF5,
    RGB_R3 = 0xFF38C7,
    RGB_R4 = 0xFF18E7,

    RGB_G  = 0xFF9A65,
    RGB_G1 = 0xFFAA55,
    RGB_G2 = 0xFF8A75,
    RGB_G3 = 0xFFB847,
    RGB_G4 = 0xFF9867,

    RGB_B  = 0xFFA25D,
    RGB_B1 = 0xFF926D,
    RGB_B2 = 0xFFB24D,
    RGB_B3 = 0xFF7887,
    RGB_B4 = 0xFF58A7,

    RGB_W  = 0xFF22DD,
    RGB_W1 = 0xFF12ED,
    RGB_W2 = 0xFF32CD,
    RGB_W3 = 0xFFF807,
    RGB_W4 = 0xFFD827,

    W_UP    = 0xFF28D7,
    W_DOWN  = 0xFFA857,
    W_OFF   = 0xFF6897,
    W_ON    = 0xFFE817,
    W_25    = 0xFF08F7,
    W_50    = 0xFF8877,
    W_75    = 0xFF48B7,
    W_100   = 0xFFC837,
    
    JUMP3 = 0xFF30CF,
    FADE3 = 0xFFB04F,
    JUMP7 = 0xFF708F,
    FADE7 = 0xFF10EF,
    FLASH = 0xFF906F,
    AUTO  = 0xFF50AF,

    QUICK = 0xFFF00F,
    SLOW  = 0xFFD02F 
};

class IR_Rec {
    public:
        IR_Rec(uint16_t pin);

        bool receive(IR_CMD *cmd);

    private:
        IRrecv rec;
};

#endif //IR_REC_H