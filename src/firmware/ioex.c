#include <xc.h>
#include "app.h"
#include "i2c_master.h"

#define IOEX_DEVICE_ADDRESS           0x20
#define IOEX_DEVICE_REGISTER_INPUT0   0b00000000
#define IOEX_DEVICE_REGISTER_INPUT1   0b00000001
#define IOEX_DEVICE_REGISTER_OUTPUT0  0b00000010
#define IOEX_DEVICE_REGISTER_OUTPUT1  0b00000011
#define IOEX_DEVICE_REGISTER_TRIS0    0b00000110
#define IOEX_DEVICE_REGISTER_TRIS1    0b00000111
#define IOEX_DEVICE_REGISTER_PULL0    0b01000110
#define IOEX_DEVICE_REGISTER_PULL1    0b01000111

#define IOEX_BITMASK_SETUP_TRIS0      0b01111111
#define IOEX_BITMASK_SETUP_TRIS1      0b10101010
#define IOEX_BITMASK_SETUP_PULL0      0b01100000
#define IOEX_BITMASK_SETUP_PULL1      0b10101010

uint8_t outState0 = 0;
uint8_t outState1 = 0;
uint8_t outTris0 = IOEX_BITMASK_SETUP_TRIS0;


void ioex_button_switch_getAll(bool* switch1, bool* switch2, bool* switch3, bool* switch4, bool* switch5) {
    uint8_t data0 = 0, data1 = 0;
    i2c_master_readRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_INPUT0, &data0, 1);
    i2c_master_readRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_INPUT1, &data1, 1);

    *switch1 = (data1 & 0b10000000) == 0;
    *switch2 = (data1 & 0b00100000) == 0;
    *switch3 = (data1 & 0b00001000) == 0;
    *switch4 = (data1 & 0b00000010) == 0;
    *switch5 = (data0 & 0b01000000) == 0;
}


void ioex_button_led_setAll(bool led1, bool led2, bool led3, bool led4, bool led5) {
    if (led1) { outState1 |= 0b01000000; } else { outState1 &= 0b10111111; }
    if (led2) { outState1 |= 0b00010000; } else { outState1 &= 0b11101111; }
    if (led3) { outState1 |= 0b00000100; } else { outState1 &= 0b11111011; }
    if (led4) { outState1 |= 0b00000001; } else { outState1 &= 0b11111110; }
    if (led5) { outState0 |= 0b10000000; } else { outState0 &= 0b01111111; }

    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT0, &outState0, 1);
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_set1(bool led1) {
    if (led1) { outState1 |= 0b01000000; } else { outState1 &= 0b10111111; }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_set2(bool led2) {
    if (led2) { outState1 |= 0b00010000; } else { outState1 &= 0b11101111; }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_set3(bool led3) {
    if (led3) { outState1 |= 0b00000100; } else { outState1 &= 0b11111011; }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_set4(bool led4) {
    if (led4) { outState1 |= 0b00000001; } else { outState1 &= 0b11111110; }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_set5(bool led5) {
    if (led5) { outState0 |= 0b10000000; } else { outState0 &= 0b01111111; }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT0, &outState0, 1);
}

void ioex_button_led_toggle1() {
    outState1 ^= 0b01000000;
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_toggle2() {
    outState1 ^= 0b00010000;
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_toggle3() {
    outState1 ^= 0b00000100;
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_toggle4() {
    outState1 ^= 0b00000001;
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}

void ioex_button_led_toggle5() {
    outState0 ^= 0b10000000;
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT0, &outState0, 1);
}


void ioex_output_checkOpenDrain(void) {
    while(true) {  // ensure output is setupp correctly to open-drain
        uint8_t state0;
        i2c_master_readRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT0, &state0, 1);
        if ((state0 & 0b00011111) == 0) { break; }  // all good

        outState0 &= 0b11100000;  // probably not needed, but let's do it anyhow
        i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT0, &outState0, 1);
    }
}

void ioex_output_setAll(bool output1, bool output2, bool output3, bool output4, bool output5) {
    // outputs are controlled via open-drain
    outTris0 = IOEX_BITMASK_SETUP_TRIS0;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
    if (!output1) { outTris0 &= 0b11111110; }
    if (!output2) { outTris0 &= 0b11111101; }
    if (!output3) { outTris0 &= 0b11111011; }
    if (!output4) { outTris0 &= 0b11110111; }
    if (!output5) { outTris0 &= 0b11101111; }

    ioex_output_checkOpenDrain();
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_TRIS0, &outTris0, 1);
}

void ioex_output_set1(bool output1) {
    if (output1) {
        outTris0 |= 0b00000001;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
        ioex_output_checkOpenDrain();
    } else {
        outTris0 &= 0b11111110;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
    }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_TRIS0, &outTris0, 1);
}

void ioex_output_set2(bool output2) {
    if (output2) {
        outTris0 |= 0b00000010;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
        ioex_output_checkOpenDrain();
    } else {
        outTris0 &= 0b11111101;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
    }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_TRIS0, &outTris0, 1);
}

void ioex_output_set3(bool output3) {
    if (output3) {
        outTris0 |= 0b00000100;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
        ioex_output_checkOpenDrain();
    } else {
        outTris0 &= 0b11111011;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
    }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_TRIS0, &outTris0, 1);
}

void ioex_output_set4(bool output4) {
    if (output4) {
        outTris0 |= 0b00001000;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
        ioex_output_checkOpenDrain();
    } else {
        outTris0 &= 0b11110111;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
    }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_TRIS0, &outTris0, 1);
}

void ioex_output_set5(bool output5) {
    if (output5) {
        outTris0 |= 0b00010000;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
        ioex_output_checkOpenDrain();
    } else {
        outTris0 &= 0b11101111;  // Led5 Switch5 - Out5 Out4 Out3 Out2 Out1
    }
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_TRIS0, &outTris0, 1);
}


void ioex_init(void) {
    uint8_t data0[2] = { IOEX_DEVICE_REGISTER_OUTPUT0, 0 };
    uint8_t data1[2] = { IOEX_DEVICE_REGISTER_OUTPUT1, 0 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, data0, 2);
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, data1, 2);

    ioex_output_checkOpenDrain();

    uint8_t pullData0[2] = { IOEX_DEVICE_REGISTER_PULL0, IOEX_BITMASK_SETUP_PULL0 };
    uint8_t pullData1[2] = { IOEX_DEVICE_REGISTER_PULL1, IOEX_BITMASK_SETUP_PULL1 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, pullData0, 2);
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, pullData1, 2);

    uint8_t trisData0[2] = { IOEX_DEVICE_REGISTER_TRIS0, IOEX_BITMASK_SETUP_TRIS0 };
    uint8_t trisData1[2] = { IOEX_DEVICE_REGISTER_TRIS1, IOEX_BITMASK_SETUP_TRIS1 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, trisData0, 2);
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, trisData1, 2);
}
