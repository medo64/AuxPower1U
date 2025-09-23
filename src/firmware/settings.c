#include <xc.h>
#include <stdint.h>
#include "nvm_eeprom.h"

uint8_t settings_outputs_get(void) {
    return nvm_eeprom_read8(1);
}

void settings_outputs_set(uint8_t value) {
    nvm_eeprom_write8(1, value);
}


void settings_cutoff_current_get(uint16_t* current1, uint16_t* current2, uint16_t* current3, uint16_t* current4, uint16_t* current5) {
    *current1 = 3600;  // 4A -10%
    *current2 = 3600;  // 4A -10%
    *current3 = 3600;  // 4A -10%
    *current4 = 5400;  // 6A -10%
    *current5 = 5400;  // 6A -10%
}


void settings_init(void) {
    if (nvm_eeprom_read8(0) != 0x2A) {  // check if there's valid data
        nvm_eeprom_write8(1, 0b11111);
        nvm_eeprom_write8(0, 0x2A);
    }
}
