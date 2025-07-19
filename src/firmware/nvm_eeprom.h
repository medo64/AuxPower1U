/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-10-08: Initial version

/**
 * Handling EEPROM reads and writes
 *
 * Defines used:
 *   _NVME_EEPROM_16:              Enable 16-bit integer read/write operation
 *   _NVME_EEPROM_24:              Enable 24-bit integer read/write operation
 *   _NVME_EEPROM_32:              Enable 32-bit integer read/write operation
 *   _NVME_EEPROM_N:               Enable multi-byte read/write operation
 */

#pragma once
#include <stdint.h>

uint8_t nvm_eeprom_read8(const uint16_t offset);
void nvm_eeprom_write8(const uint16_t offset, const uint8_t value);

#if defined(_NVME_EEPROM_16)
    uint16_t nvm_eeprom_read16(const uint16_t offset);
    void nvm_eeprom_write16(const uint16_t offset, const uint16_t value);
#endif

#if defined(_NVME_EEPROM_24)
    uint24_t nvm_eeprom_read24(const uint16_t offset);
    void nvm_eeprom_write24(const uint16_t offset, const uint24_t value);
#endif

#if defined(_NVME_EEPROM_32)
    uint32_t nvm_eeprom_read32(const uint16_t offset);
    void nvm_eeprom_write32(const uint16_t offset, const uint32_t value);
#endif

#if defined(_NVME_EEPROM_N) || defined(_NVME_EEPROM_16) || defined(_NVME_EEPROM_24) || defined(_NVME_EEPROM_32)
    void nvm_eeprom_readData(const uint16_t offset, uint8_t* data, const uint8_t count);
    void nvm_eeprom_writeData(const uint16_t offset, const uint8_t* data, const uint8_t count);
#endif
