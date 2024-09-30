#pragma once
#include <stdbool.h>

void ioex_init(void);

void ioex_button_switch_getAll(bool* switch1, bool* switch2, bool* switch3, bool* switch4, bool* switch5);

void ioex_button_led_setAll(bool led1, bool led2, bool led3, bool led4, bool led5);
void ioex_button_led_set1(bool led1);
void ioex_button_led_set2(bool led2);
void ioex_button_led_set3(bool led3);
void ioex_button_led_set4(bool led4);
void ioex_button_led_set5(bool led5);

void ioex_output_setAll(bool output1, bool output2, bool output3, bool output4, bool output5);
void ioex_output_set1(bool output1);
void ioex_output_set2(bool output2);
void ioex_output_set3(bool output3);
void ioex_output_set4(bool output4);
void ioex_output_set5(bool output5);
