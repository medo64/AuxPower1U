/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */

#include <xc.h>
#include <stdint.h>
#include "app.h"
#include "ssd1306.h"
#include "ssd1306_font.h"
#include "i2c_master.h"

#define SSD1306_SET_LOWER_START_COLUMN_ADDRESS       0x00
#define SSD1306_SET_UPPER_START_COLUMN_ADDRESS       0x10
#define SSD1306_SET_MEMORY_ADDRESSING_MODE           0x20
#define SSD1306_SET_COLUMN_ADDRESS                   0x21
#define SSD1306_SET_PAGE_ADDRESS                     0x22
#define SSD1306_SET_DISPLAY_START_LINE               0x40
#define SSD1306_SET_CONTRAST_CONTROL                 0x81
#define SSD1306_SET_CHARGE_PUMP                      0x8D
#define SSD1306_SET_SEGMENT_REMAP_COL0               0xA0
#define SSD1306_SET_SEGMENT_REMAP_COL127             0xA1
#define SSD1306_ENTIRE_DISPLAY_ON                    0xA4
#define SSD1306_ENTIRE_DISPLAY_ON_FORCED             0xA5
#define SSD1306_SET_NORMAL_DISPLAY                   0xA6
#define SSD1306_SET_INVERSE_DISPLAY                  0xA7
#define SSD1306_SET_DISPLAY_OFF                      0xAE
#define SSD1306_SET_DISPLAY_ON                       0xAF
#define SSD1306_SET_MULTIPLEX_RATIO                  0xA8
#define SSD1306_SET_PAGE_START_ADDRESS               0xB0
#define SSD1306_SET_COM_OUTPUT_SCAN_DIRECTION_INC    0xC0
#define SSD1306_SET_COM_OUTPUT_SCAN_DIRECTION_DEC    0xC8
#define SSD1306_SET_DISPLAY_OFFSET                   0xD3
#define SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO       0xD5
#define SSD1306_SET_PRECHARGE_PERIOD                 0xD9
#define SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION  0xDA
#define SSD1306_SET_VCOMH_DESELECT_LEVEL             0xDB

void writeRawCommand1(const uint8_t datum1);
void writeRawCommand2(const uint8_t datum1, const uint8_t datum2);
void writeRawData(const uint8_t* data, const uint8_t count);
void writeRawDataZeros(const uint8_t count);


#if defined(_SSD1306_CUSTOM_INIT)
    uint8_t displayAddress;
    uint8_t displayWidth;
    uint8_t displayHeight;
    uint8_t displayColumns;
    uint8_t displayRows;
#else
    uint8_t displayAddress = _SSD1306_DISPLAY_ADDRESS;
    uint8_t displayWidth = _SSD1306_DISPLAY_WIDTH;
    uint8_t displayHeight = _SSD1306_DISPLAY_HEIGHT;
    uint8_t displayColumns = (_SSD1306_DISPLAY_WIDTH / 8);
    uint8_t displayRows = (_SSD1306_DISPLAY_HEIGHT / 8);
#endif

uint8_t currentRow;
uint8_t currentColumn;

void ssd1306_internalInit() {
    writeRawCommand1(SSD1306_SET_DISPLAY_OFF);                                    // Set Display Off
    writeRawCommand2(SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO, 0x80);               // Set Display Clock Divide Ratio/Oscillator Frequency
    writeRawCommand2(SSD1306_SET_MULTIPLEX_RATIO, displayHeight - 1);             // Set Multiplex Ratio (line count - 1)
    writeRawCommand2(SSD1306_SET_DISPLAY_OFFSET, 0x00);                           // Set Display Offset
    writeRawCommand1(SSD1306_SET_DISPLAY_START_LINE);                             // Set Display Start Line 
    writeRawCommand2(SSD1306_SET_CHARGE_PUMP, 0x14);                              // Set Charge Pump (0x10 external vcc, 0x14 internal vcc)
    #if defined(_SSD1306_DISPLAY_FLIP)
        writeRawCommand1(SSD1306_SET_SEGMENT_REMAP_COL127);                       // Set Segment Re-Map
        writeRawCommand1(SSD1306_SET_COM_OUTPUT_SCAN_DIRECTION_DEC);              // Set COM Output Scan Direction
    #else
        writeRawCommand1(SSD1306_SET_SEGMENT_REMAP_COL0);                         // Set Segment Re-Map
        writeRawCommand1(SSD1306_SET_COM_OUTPUT_SCAN_DIRECTION_INC);              // Set COM Output Scan Direction
    #endif
    if (displayHeight == 32) {
        writeRawCommand2(SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION, 0x02);      // Set COM Pins Hardware Configuration (0x02 128x32)
        writeRawCommand2(SSD1306_SET_CONTRAST_CONTROL, 0x8F);                     // Set Contrast Control (0x8F 128x32; external vcc; internal vcc)
    } else {
        writeRawCommand2(SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION, 0x12);      // Set COM Pins Hardware Configuration (0x12 128x64)
        writeRawCommand2(SSD1306_SET_CONTRAST_CONTROL, 0xCF);                     // Set Contrast Control (0xCF 128x64; external vcc; internal vcc)
    }
    writeRawCommand2(SSD1306_SET_PRECHARGE_PERIOD, 0xF1);                         // Set Pre-Charge Period (0x22 external vcc; 0xF1 internal vcc)
    writeRawCommand2(SSD1306_SET_VCOMH_DESELECT_LEVEL, 0x40);                     // Set VCOMH Deselect Level
    writeRawCommand1(SSD1306_ENTIRE_DISPLAY_ON);                                  // Set Entire Display On/Off
    writeRawCommand1(SSD1306_SET_NORMAL_DISPLAY);                                 // Set Normal Display

    writeRawCommand2(SSD1306_SET_MEMORY_ADDRESSING_MODE, 0b10);                   // Set Page addressing mode

    ssd1306_clearAll();

    writeRawCommand1(SSD1306_SET_DISPLAY_ON);                                     // Set Display On
}

#if defined(_SSD1306_CUSTOM_INIT)
    void ssd1306_init(const uint8_t address, const uint8_t width, const uint8_t height) {
        displayAddress = address;
        displayWidth = width;
        displayHeight = height;
        displayColumns = width / 8;
        displayRows = height / 8;
        ssd1306_internalInit();
    }
#else
    void ssd1306_init(void) {
        ssd1306_internalInit();
    }
#endif


#if defined(_SSD1306_CONTROL_DISPLAY)
    void ssd1306_displayOff(void) {
        writeRawCommand1(SSD1306_SET_DISPLAY_OFF);
    }

    void ssd1306_displayOn(void) {
        writeRawCommand1(SSD1306_SET_DISPLAY_ON);
    }
#endif

#if defined(_SSD1306_CONTROL_INVERT)
    void ssd1306_displayInvert(void) {
        writeRawCommand1(SSD1306_SET_INVERSE_DISPLAY);
    }

    void ssd1306_displayNormal(void) {
        writeRawCommand1(SSD1306_SET_NORMAL_DISPLAY);
    }
#endif

#if defined(_SSD1306_CONTROL_FLIP)
    void ssd1306_displayFlip(bool flipped) {
        if (flipped) {
            writeRawCommand1(SSD1306_SET_SEGMENT_REMAP_COL127);                       // Set Segment Re-Map
            writeRawCommand1(SSD1306_SET_COM_OUTPUT_SCAN_DIRECTION_DEC);              // Set COM Output Scan Direction
        } else {
            writeRawCommand1(SSD1306_SET_SEGMENT_REMAP_COL0);                         // Set Segment Re-Map
            writeRawCommand1(SSD1306_SET_COM_OUTPUT_SCAN_DIRECTION_INC);              // Set COM Output Scan Direction
        }
    }
#endif

#if defined(_SSD1306_CONTROL_CONTRAST)
    void ssd1306_setContrast(const uint8_t value) {
        writeRawCommand2(SSD1306_SET_CONTRAST_CONTROL, value);
    }
#endif

void ssd1306_clearAll(void) {
    for (uint8_t i = 0; i < displayRows; i++) {
        writeRawCommand1(SSD1306_SET_PAGE_START_ADDRESS | i);
        writeRawDataZeros(displayWidth);
    }
    ssd1306_moveTo(1, 1);
}


#if defined(_SSD1306_FONT_8x8)
    void ssd1306_clearRemaining(void) {
        uint8_t columnCount = (uint8_t)((displayColumns - currentColumn) << 3);
        uint8_t nextColumn = currentColumn + 1;
        uint8_t nextRow = currentRow + 1;
        writeRawDataZeros(columnCount);
        ssd1306_moveTo(nextRow, nextColumn);
    }
#endif

#if defined(_SSD1306_FONT_8x16)
    void ssd1306_clearRemaining16(void) {
        uint8_t columnCount = (uint8_t)((displayColumns - currentColumn) << 3);
        uint8_t nextColumn = currentColumn + 1;
        uint8_t nextRow = currentRow + 1;
        writeRawDataZeros(columnCount);
        ssd1306_moveTo(nextRow + 1, nextColumn);
        writeRawDataZeros(columnCount);
        ssd1306_moveTo(nextRow, nextColumn);
    }
#endif


#if defined(_SSD1306_FONT_8x8)
    bool ssd1306_clearRow(const uint8_t row) {
        if (ssd1306_moveTo(row, 1)) {
            writeRawDataZeros(displayWidth);
            return true;
        }
        return false;
    }
#endif

#if defined(_SSD1306_FONT_8x16)
    bool ssd1306_clearRow16(const uint8_t row) {
        if (ssd1306_moveTo(row, 1)) {
            writeRawDataZeros(displayWidth);
            if (ssd1306_moveTo(row + 1, 1)) {
                writeRawDataZeros(displayWidth);
                return true;
            }
        }
        return false;
    }
#endif


bool ssd1306_moveTo(const uint8_t row, const uint8_t column) {
    if ((row <= displayRows) && (column <= displayColumns)) {
        uint8_t newRow = (row == 0) ? currentRow : row - 1;
        uint8_t newColumn = (column == 0) ? currentColumn : column - 1;
        uint8_t newColumnL = (newColumn << 3) & 0x0F;
        uint8_t newColumnH = (newColumn >> 1) & 0x0F;
        writeRawCommand1(SSD1306_SET_PAGE_START_ADDRESS | newRow);
        writeRawCommand1(SSD1306_SET_LOWER_START_COLUMN_ADDRESS | newColumnL);
        writeRawCommand1(SSD1306_SET_UPPER_START_COLUMN_ADDRESS | newColumnH);
        currentRow = newRow;
        currentColumn = newColumn;
        return true;
    }
    return false;
}


#if defined(_SSD1306_FONT_8x8)
    bool ssd1306_moveToNextRow(void) {
        if (currentRow >= displayRows - 1) { return false; }
        uint8_t newRow = currentRow + 1;
        writeRawCommand1(SSD1306_SET_PAGE_START_ADDRESS | newRow);
        writeRawCommand1(SSD1306_SET_LOWER_START_COLUMN_ADDRESS);
        writeRawCommand1(SSD1306_SET_UPPER_START_COLUMN_ADDRESS);
        currentRow = newRow;
        currentColumn = 0;
        return true;
    }
#endif

#if defined(_SSD1306_FONT_8x16)
    bool ssd1306_moveToNextRow16(void) {
        if (currentRow >= displayRows - 1) { return false; }
        uint8_t newRow = currentRow + 2;
        writeRawCommand1(SSD1306_SET_PAGE_START_ADDRESS | newRow);
        writeRawCommand1(SSD1306_SET_LOWER_START_COLUMN_ADDRESS);
        writeRawCommand1(SSD1306_SET_UPPER_START_COLUMN_ADDRESS);
        currentRow = newRow;
        currentColumn = 0;
        return true;
    }
#endif


bool ssd1306_drawCharacter(const uint8_t* data, const uint8_t count) {
    if ((count != 8) && (count != 16)) { return false; }  // must be 8x8 or 8x16 pixels - 8/16 bytes
    if (currentColumn >= displayColumns) { return false; }

    if (count >= 16) {
        writeRawCommand1(SSD1306_SET_PAGE_START_ADDRESS | (currentRow + 1));
        writeRawData(data + 8, 8);

        uint8_t currentColumnLow = (currentColumn << 3) & 0x0F;
        uint8_t currentColumnHigh = (currentColumn >> 1) & 0x0F;
        writeRawCommand1(SSD1306_SET_PAGE_START_ADDRESS | currentRow);
        writeRawCommand1(SSD1306_SET_LOWER_START_COLUMN_ADDRESS | currentColumnLow);
        writeRawCommand1(SSD1306_SET_UPPER_START_COLUMN_ADDRESS | currentColumnHigh);
        writeRawData(data, 8);
    } else {
        writeRawData(data, 8);
    }
    currentColumn++;

    return true;
}

#if defined(_SSD1306_FONT_8x8)
bool ssd1306_writeCharacter(const char value) {
    if (value < 32) {
#if defined(_SSD1306_FONT_8x8_LOW)
        uint16_t offset = (uint16_t)(value << 3);  // *8
        return ssd1306_drawCharacter(&font_low_8x8[offset], 8);
#else
        return ssd1306_drawCharacter(&font_basic_8x8[0], 8);
#endif
    } else if (value > 126) {
#if defined(_SSD1306_FONT_8x8_HIGH)
        uint16_t offset = (uint16_t)((value - 127) << 3);  // *8
        return ssd1306_drawCharacter(&font_high_8x8[offset], 8);
#else
        return ssd1306_drawCharacter(&font_basic_8x8[0], 8);
#endif
    } else {
        uint16_t offset = (uint16_t)((value - 32) << 3);  // *8
        return ssd1306_drawCharacter(&font_basic_8x8[offset], 8);
    }
}
#endif

#if defined(_SSD1306_FONT_8x16)
bool ssd1306_writeCharacter16(const char value) {
    if (value < 32) {
        #if defined(_SSD1306_FONT_8x16_LOW)
            uint16_t offset = (uint16_t)(value << 4);  // *16
            return ssd1306_drawCharacter(&font_low_8x16[offset], 16);
        #else
            return ssd1306_drawCharacter(&font_basic_8x16[0], 16);
        #endif
    } else if (value > 126) {
        #if defined(_SSD1306_FONT_8x16_HIGH)
            uint16_t offset = (uint16_t)((value - 127) << 4);  // *16
            return ssd1306_drawCharacter(&font_high_8x16[offset], 16);
        #else
            return ssd1306_drawCharacter(&font_basic_8x16[0], 16);
        #endif
    } else {
        uint16_t offset = (uint16_t)((value - 32) << 4);  // *16
        return ssd1306_drawCharacter(&font_basic_8x16[offset], 16);
    }
}
#endif

#if defined(_SSD1306_FONT_8x8)
    bool ssd1306_writeText(const char* text) {
        bool ok = true;
        while (*text != 0) {
            ok &= ssd1306_writeCharacter(*text);
            text++;
        }
        return ok;
    }

    bool ssd1306_writeLine(const char* text) {
        bool ok = ssd1306_writeText(text);
        ssd1306_moveToNextRow();
        return ok;
    }
#endif


#if defined(_SSD1306_FONT_8x16)
    bool ssd1306_writeText16(const char* text) {
        bool ok = true;
        while (*text != 0) {
            ok &= ssd1306_writeCharacter16(*text);
            text++;
        }
        return ok;
    }

    bool ssd1306_writeLine16(const char* text) {
        bool ok = ssd1306_writeText16(text);
        ssd1306_moveToNextRow16();
        return ok;
    }
#endif



void writeRawCommand1(const uint8_t datum1) {
    i2c_master_writeRegisterBytes(displayAddress, 0x00, &datum1, 1);
}

void writeRawCommand2(const uint8_t datum1, const uint8_t datum2) {
    uint8_t data[2] = { datum1, datum2 };
    i2c_master_writeRegisterBytes(displayAddress, 0x00, data, 2);
}

void writeRawData(const uint8_t *data, const uint8_t count) {
    i2c_master_writeRegisterBytes(displayAddress, 0x40, data, count);
}

void writeRawDataZeros(const uint8_t count) {
    i2c_master_writeRegisterZeroBytes(displayAddress, 0x40, count);
}
