#pragma once
#include <stdbool.h>

void ioex_init(void);

void ioex_button_getSwitches(bool* switch1, bool* switch2, bool* switch3, bool* switch4, bool* switch5);
void ioex_button_setLeds(bool led1, bool led2, bool led3, bool led4, bool led5);
void ioex_button_setOutputs(bool output1, bool output2, bool output3, bool output4, bool output5);
