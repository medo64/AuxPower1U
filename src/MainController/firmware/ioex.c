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

void ioex_setOutput() {
    uint8_t data0[2] = { IOEX_DEVICE_REGISTER_OUTPUT0, outState0 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, data0, 2);
    uint8_t data1[2] = { IOEX_DEVICE_REGISTER_OUTPUT1, outState1 };
    i2c_master_writeBytes(IOEX_DEVICE_ADDRESS, data1, 2);
}


void ioex_button1_led_on(void) {
    outState1 |= 0b01000000;
    ioex_setOutput();
}

void ioex_button1_led_off(void) {
    outState1 &= 0b10111111;
    ioex_setOutput();
}


void ioex_button2_led_on(void) {
    outState1 |= 0b00010000;
    ioex_setOutput();
}

void ioex_button2_led_off(void) {
    outState1 &= 0b11101111;
    ioex_setOutput();
}


void ioex_button3_led_on(void) {
    outState1 |= 0b00000100;
    ioex_setOutput();
}

void ioex_button3_led_off(void) {
    outState1 &= 0b11111011;
    ioex_setOutput();
}


void ioex_button4_led_on(void) {
    outState1 |= 0b00000001;
    ioex_setOutput();
}

void ioex_button4_led_off(void) {
    outState1 &= 0b11111110;
    ioex_setOutput();
}


void ioex_button5_led_on(void) {
    outState0 |= 0b10000000;
    ioex_setOutput();
}

void ioex_button5_led_off(void) {
    outState0 &= 0b01111111;
    ioex_setOutput();
}


void ioex_output1_on(void) {
    outState0 |= 0b00000001;
    ioex_setOutput();
}

void ioex_output1_off(void) {
    outState0 &= 0b11111110;
    ioex_setOutput();
}


void ioex_output2_on(void) {
    outState0 |= 0b00000010;
    ioex_setOutput();
}

void ioex_output2_off(void) {
    outState0 &= 0b11111101;
    ioex_setOutput();
}


void ioex_output3_on(void) {
    outState0 |= 0b00000100;
    ioex_setOutput();
}

void ioex_output3_off(void) {
    outState0 &= 0b11111011;
    ioex_setOutput();
}


void ioex_output4_on(void) {
    outState0 |= 0b00001000;
    ioex_setOutput();
}

void ioex_output4_off(void) {
    outState0 &= 0b11110111;
    ioex_setOutput();
}


void ioex_output5_on(void) {
    outState0 |= 0b00010000;
    ioex_setOutput();
}

void ioex_output5_off(void) {
    outState0 &= 0b11101111;
    ioex_setOutput();
}


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
