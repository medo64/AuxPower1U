/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-10-15: Adjusted for 128x128 display
// 2024-09-26: Added inverse writing
//             Added progress bar
// 2024-09-23: Always uses external master
// 2024-09-02: Refactored to work with OLED firmware
// 2023-10-10: Expanded functionality
// 2023-10-14: Internal I2C support

/**
 * Handling SSD1306 display output
 *
 * Defines used:
 *   _SSD1306_DISPLAY_ADDRESS <N>: I2C address; default is 0x3C
 *   _SSD1306_DISPLAY_HEIGHT <N>:  Display height; can be 32, 64, or 128
 *   _SSD1306_DISPLAY_WIDTH <N>:   Display width; can be 64 or 128
 *   _SSD1306_DISPLAY_FLIP:        Flips screen to other direction
 *   _SSD1306_FONT_8x8:            Use 8-pixel high text
 *   _SSD1306_FONT_8x8_LOW:        Include lower 32 ASCII control characters
 *   _SSD1306_FONT_8x8_HIGH:       Include upper 128 CP437 ASCII characters
 *   _SSD1306_FONT_8x16:           Use 16-pixel high text
 *   _SSD1306_FONT_8x16_LOW:       Include lower 32 ASCII control characters
 *   _SSD1306_FONT_8x16_HIGH:      Include upper 128 CP437 ASCII characters
 *   _SSD1306_WRITE_INVERSE:       Allows inverse writes
 *   _SSD1306_WRITE_PROGRESS:      Allows writing progress bar
 *   _SSD1306_CONTROL_DISPLAY:     Allows display control (displayOff, displayOn)
 *   _SSD1306_CONTROL_INVERT:      Allows display control (displayInvert, displayNormal)
 *   _SSD1306_CONTROL_FLIP:        Allows display control (displayFlip)
 *   _SSD1306_CONTROL_CONTRAST:    Allows contrast control (setContrast)
 *   _SSD1306_CUSTOM_INIT:         Uses customizable initialization function
 */

#pragma once

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"

#if !defined(_SSD1306_FONT_8x8) & !defined(_SSD1306_FONT_8x16)  // at least one font has to be supported
    #define _SSD1306_FONT_8x8
#endif


/** Initializes Display. */
#if defined(_SSD1306_CUSTOM_INIT)
    void ssd1306_init(const uint8_t address, const uint8_t width, const uint8_t height);
#else
    void ssd1306_init(void);

    #if !defined(_SSD1306_DISPLAY_ADDRESS)
        #define _SSD1306_DISPLAY_ADDRESS  0x3C
    #endif

    #if !defined(_SSD1306_DISPLAY_HEIGHT)
        #define _SSD1306_DISPLAY_HEIGHT   32
    #elif (_SSD1306_DISPLAY_HEIGHT != 32) && (_SSD1306_DISPLAY_HEIGHT != 64) && (_SSD1306_DISPLAY_HEIGHT != 128)
        #error SSD1306 display height not supported
    #endif

    #if !defined(_SSD1306_DISPLAY_WIDTH)
        #define _SSD1306_DISPLAY_WIDTH   128
    #elif (_SSD1306_DISPLAY_WIDTH != 64) && (_SSD1306_DISPLAY_WIDTH != 128)
        #error SSD1306 display width not supported
    #endif
#endif

/** Turns display off. */
#if defined(_SSD1306_CONTROL_DISPLAY)
    void ssd1306_displayOff(void);
#endif

/** Turns display on. */
#if defined(_SSD1306_CONTROL_DISPLAY)
    void ssd1306_displayOn(void);
#endif

/** Turns on display inversion. */
#if defined(_SSD1306_CONTROL_INVERT)
    void ssd1306_displayInvert(void);
#endif

/** Turns off display inversion. */
#if defined(_SSD1306_CONTROL_INVERT)
    void ssd1306_displayNormal(void);
#endif

/** Flips display orientation. */
#if defined(_SSD1306_CONTROL_FLIP)
    void ssd1306_displayFlip(bool flipped);
#endif

/** Sets contrast value. */
#if defined(_SSD1306_CONTROL_CONTRAST)
    void ssd1306_setContrast(const uint8_t value);
#endif

/** Sets column and row to be used (at 8x8 resolution). */
bool ssd1306_moveTo(const uint8_t row, const uint8_t column);


/** Moves cursor to the first column of the next row. */
#if defined(_SSD1306_FONT_8x8)
    bool ssd1306_moveToNextRow(void);
#endif

/** Moves cursor to the first column of the next row (16-pixel characters). */
#if defined(_SSD1306_FONT_8x16)
    bool ssd1306_moveToNextRow16(void);
#endif


/** Clear display content. */
void ssd1306_clearAll(void);


/** Clear remaining. */
#if defined(_SSD1306_FONT_8x8)
    void ssd1306_clearRemaining(void);
#endif

/** Clear remaining. */
#if defined(_SSD1306_FONT_8x16)
    void ssd1306_clearRemaining16(void);
#endif


/** Clear display content of a single row. */
#if defined(_SSD1306_FONT_8x8)
    bool ssd1306_clearRow(const uint8_t row);
#endif

/** Clear display content of a single row. */
#if defined(_SSD1306_FONT_8x16)
    bool ssd1306_clearRow16(const uint8_t row);
#endif


/** Writes custom 8x8 character at the current position from 8 bytes given. */
bool ssd1306_drawCustom(const uint8_t* data);

#if defined(_SSD1306_FONT_8x8)
    /** Writes 8x8 character at the current position */
    bool ssd1306_writeCharacter(const char value);

    #if defined _SSD1306_WRITE_INVERSE
        /** Writes inverse 8x8 character at the current position */
        bool ssd1306_writeInverseCharacter(const char value);
    #endif
#endif

#if defined(_SSD1306_FONT_8x16)
    /** Writes custom 8x16 character at the current position from 8 bytes given. */
    bool ssd1306_drawCustom16(const uint8_t* data);

    /** Writes 8x16 character at the current position */
    bool ssd1306_writeCharacter16(const char value);

    #if defined _SSD1306_WRITE_INVERSE
        /** Writes inverse 8x16 character at the current position */
        bool ssd1306_writeInverseCharacter16(const char value);
    #endif
#endif


#if defined(_SSD1306_FONT_8x8)
    /** Writes 8x8 text at the current position */
    bool ssd1306_writeText(const char* text);

    #if defined _SSD1306_WRITE_INVERSE
        /** Writes inverse 8x8 text at the current position */
        bool ssd1306_writeInverseText(const char* text);
    #endif
#endif

#if defined(_SSD1306_FONT_8x8)
    /** Writes 8x8 text at the current position and moves to the next line */
    bool ssd1306_writeLine(const char* text);

    #if defined _SSD1306_WRITE_INVERSE
        /** Writes inverse 8x8 text at the current position and moves to the next line */
        bool ssd1306_writeInverseLine(const char* text);
    #endif
#endif

#if defined(_SSD1306_FONT_8x16)
    /** Writes 8x16 text at the current position */
    bool ssd1306_writeText16(const char* text);

    #if defined _SSD1306_WRITE_INVERSE
        /** Writes inverse 8x16 text at the current position */
        bool ssd1306_writeInverseText16(const char* text);
    #endif
#endif

#if defined(_SSD1306_FONT_8x16)
    /** Writes 8x16 text at the current position and moves to the next line */
    bool ssd1306_writeLine16(const char* text);

    #if defined _SSD1306_WRITE_INVERSE
        /** Writes inverse 8x16 text at the current position and moves to the next line */
        bool ssd1306_writeInverseLine16(const char* text);
    #endif
#endif


#if defined(_SSD1306_WRITE_PROGRESS)
    /** Writes progress bar with a given value */
    bool ssd1306_writeProgress(const uint8_t characterCount, const uint8_t percentValue);
#endif
