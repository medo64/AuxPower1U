#include <stdint.h>

uint8_t settings_outputs_get(void) {
    return 0b11111;  // TODO: read from EEPROM
}

void settings_outputs_set(uint8_t value) {
    //TODO: write to EEPROM
}


void settings_init(void) {
    // TODO: initialize EEPROM
}
