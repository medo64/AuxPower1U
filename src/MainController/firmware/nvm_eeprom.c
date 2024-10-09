#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "nvm_eeprom.h"

uint8_t nvm_eeprom_read8(const uint16_t offset) {
    NVMCON1bits.REG = 0b00;  // access data EEPROM memory locations
    NVMADRH = offset >> 8;
    NVMADRL = (uint8_t)offset;

    NVMCON1bits.RD = 1; // initiates an EEPROM read
    return NVMDAT;
}

void nvm_eeprom_write8(const uint16_t offset, const uint8_t value) {
    bool oldGIE = INTCON0bits.GIE;  // save GIE state
    INTCON0bits.GIE = 0;  // disable interrupts

    NVMCON1bits.REG = 0b00;  // access data EEPROM memory locations
    NVMCON1bits.WREN = 1;  // enable write

    NVMADRH = offset >> 8;
    NVMADRL = (uint8_t)offset;

    NVMDAT = value;

    // write sequence unlock
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;  // initiates an EEPROM write

    while(NVMCON1bits.WR);  // waits for write cycle to complete

    NVMCON1bits.WREN = 0;  // disable write
    INTCON0bits.GIE = oldGIE;  // restore interrupts
}


#if defined(_NVME_EEPROM_16)
    uint16_t nvm_eeprom_read16(const uint16_t offset) {
        uint16_t result;
        nvm_eeprom_readData(offset, (uint8_t*)&result, 2);
        return result;
    }

    void nvm_eeprom_write16(const uint16_t offset, const uint16_t value) {
        nvm_eeprom_writeData(offset, (uint8_t*)&value, 2);
    }
#endif


#if defined(_NVME_EEPROM_24)
    uint24_t nvm_eeprom_read24(const uint16_t offset) {
        uint24_t result;
        nvm_eeprom_readData(offset, (uint8_t*)&result, 3);
        return result;
    }

    void nvm_eeprom_write24(const uint16_t offset, const uint24_t value) {
        nvm_eeprom_writeData(offset, (uint8_t*)&value, 3);
    }
#endif


#if defined(_NVME_EEPROM_32)
    uint32_t nvm_eeprom_read32(const uint16_t offset) {
        uint32_t result;
        nvm_eeprom_readData(offset, (uint8_t*)&result, 4);
        return result;
    }

    void nvm_eeprom_write32(const uint16_t offset, const uint32_t value) {
        nvm_eeprom_writeData(offset, (uint8_t*)&value, 4);
    }
#endif


#if defined(_NVME_EEPROM_N) || defined(_NVME_EEPROM_16) || defined(_NVME_EEPROM_24) || defined(_NVME_EEPROM_32)

    void nvm_eeprom_readData(const uint16_t offset, uint8_t* data, const uint8_t count) {
        NVMCON1bits.REG = 0b00;  // access data EEPROM memory locations

        uint8_t remaining = count;
        uint16_t addr = offset;
        while(remaining > 0) {
            NVMADRH = (uint8_t)(addr >> 8);
            NVMADRL = (uint8_t)addr;

            NVMCON1bits.RD = 1; // initiates an EEPROM read
            *data = NVMDAT;

            addr++;
            data++;
            remaining--;
        }
    }

    void nvm_eeprom_writeData(const uint16_t offset, const uint8_t* data, const uint8_t count) {
        bool oldGIE = INTCON0bits.GIE;  // save GIE state
        INTCON0bits.GIE = 0;  // disable interrupts

        NVMCON1bits.REG = 0b00;  // access data EEPROM memory locations
        NVMCON1bits.WREN = 1;  // enable write

        uint16_t addr = offset;
        uint8_t remaining = count;
        while(remaining > 0) {
            NVMADRH = (uint8_t)(addr >> 8);
            NVMADRL = (uint8_t)addr;

            NVMDAT = *data;

            // write sequence unlock
            NVMCON2 = 0x55;
            NVMCON2 = 0xAA;

            NVMCON1bits.WR = 1;  // initiates an EEPROM write
            while(NVMCON1bits.WR);  // waits for write cycle to complete

            addr++;
            data++;
            remaining--;
        }

        NVMCON1bits.WREN = 0;  // disable write
        INTCON0bits.GIE = oldGIE;  // restore interrupts
    }

#endif
