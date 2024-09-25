#include <xc.h>
#include "i2c_master.h"

#define IOEX_DEVICE_ADDRESS           0x20
#define IOEX_DEVICE_REGISTER_INPUT0   0b00000000
#define IOEX_DEVICE_REGISTER_INPUT1   0b00000001
#define IOEX_DEVICE_REGISTER_OUTPUT0  0b00000010
#define IOEX_DEVICE_REGISTER_OUTPUT1  0b00000011
#define IOEX_DEVICE_REGISTER_CONFIG0  0b00000110
#define IOEX_DEVICE_REGISTER_CONFIG1  0b00000111
#define IOEX_DEVICE_REGISTER_PULL0    0b01000110
#define IOEX_DEVICE_REGISTER_PULL1    0b01000111

uint8_t outState0 = 0;
uint8_t outState1 = 0;


void ioex_button_getSwitches(bool* switch1, bool* switch2, bool* switch3, bool* switch4, bool* switch5) {
    uint8_t data0 = 0, data1 = 0;
    i2c_master_readRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_INPUT0, &data0, 1);
    i2c_master_readRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_INPUT1, &data1, 1);

    *switch1 = (data1 & 0b10000000) == 0;
    *switch2 = (data1 & 0b00100000) == 0;
    *switch3 = (data1 & 0b00001000) == 0;
    *switch4 = (data1 & 0b00000010) == 0;
    *switch5 = (data0 & 0b01000000) == 0;
}

void ioex_button_setLeds(bool led1, bool led2, bool led3, bool led4, bool led5) {
    if (led1) { outState1 |= 0b01000000; } else { outState1 &= 0b10111111; }
    if (led2) { outState1 |= 0b00010000; } else { outState1 &= 0b11101111; }
    if (led3) { outState1 |= 0b00000100; } else { outState1 &= 0b11111011; }
    if (led4) { outState1 |= 0b00000001; } else { outState1 &= 0b11111110; }
    if (led5) { outState0 |= 0b10000000; } else { outState0 &= 0b01111111; }

    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT0, &outState0, 1);
    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT1, &outState1, 1);
}


void ioex_button_setOutputs(bool output1, bool output2, bool output3, bool output4, bool output5) {
    if (output1) { outState0 |= 0b00000001; } else { outState0 &= 0b11111110; }
    if (output2) { outState0 |= 0b00000010; } else { outState0 &= 0b11111101; }
    if (output3) { outState0 |= 0b00000100; } else { outState0 &= 0b11111011; }
    if (output4) { outState0 |= 0b00001000; } else { outState0 &= 0b11110111; }
    if (output5) { outState0 |= 0b00010000; } else { outState0 &= 0b11101111; }

    i2c_master_writeRegisterBytes(IOEX_DEVICE_ADDRESS, IOEX_DEVICE_REGISTER_OUTPUT0, &outState0, 1);
}


void ioex_init(void) {
    uint8_t configData0[2] = { IOEX_DEVICE_REGISTER_CONFIG0, 0b01100000 };
    uint8_t configData1[2] = { IOEX_DEVICE_REGISTER_CONFIG1, 0b10101010 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, configData0, 2);
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, configData1, 2);

    uint8_t pullData0[2] = { IOEX_DEVICE_REGISTER_PULL0, 0b01100000 };
    uint8_t pullData1[2] = { IOEX_DEVICE_REGISTER_PULL1, 0b10101010 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, pullData0, 2);
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, pullData1, 2);

    uint8_t data0[2] = { IOEX_DEVICE_REGISTER_OUTPUT0, 0 };
    uint8_t data1[2] = { IOEX_DEVICE_REGISTER_OUTPUT1, 0 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, data0, 2);
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, data1, 2);
}
