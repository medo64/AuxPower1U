#include <xc.h>
#include "app.h"

#define ADC_CURRENT_OUT_MULTIPLIER       34
#define ADC_VOLTAGE_OUT_MULTIPLIER       16  // 32:1 / 2
#define ADC_VOLTAGE_12V_MULTIPLIER    8 / 2  // 8:1 / 2
#define ADC_TEMPERATURE_0C              800  // 400mV / 2048mV * 4096
#define ADC_TEMPERATURE_BITS_PER_10THC    5  // 2048mV / 4096 * 10
#define TS02_Mv                        3684  // Temperature Indicator Voltage Sensitivity

uint16_t DiaTSHR2;
uint16_t DiaFVRA2X;

void adc_init(void) {
    // Temperature Indicator ADC reading at @ 90°C (high range setting)
    TBLPTR = DIA_TSHR2;
    asm("TBLRD*+");
    DiaTSHR2 = TABLAT;
    asm("TBLRD*");
    DiaTSHR2 |= (uint16_t)TABLAT << 8;

    // ADC FVR1 Output Voltage for 2x
    TBLPTR = DIA_FVRA2X;
    asm("TBLRD*+");
    DiaFVRA2X = TABLAT;
    asm("TBLRD*");
    DiaFVRA2X |= (uint16_t)TABLAT << 8;

    TRISA  |= 0b11111111;
    ANSELA |= 0b11111111;
    TRISC  |= 0b00001111;
    ANSELC |= 0b00001111;

    // FVR @ 2.048
    FVRCONbits.ADFVR = 0b10;  // FVR Buffer 1 Gain is 2x, (2.048V)
    FVRCONbits.CDAFVR = 0b11; // FVR Buffer 2 Gain is 4x, (4.096V) - also used for ADC channel
    FVRCONbits.EN = 1;        // enable FVR
    while (!FVRCONbits.RDY);  // wait for ready
    ADREFbits.PREF = 0b11;    // Vref+ is connected to internal Fixed Voltage Reference (FVR) module

    // Temperature indicator
    FVRCONbits.TSRNG = 1;     // High Range
    FVRCONbits.TSEN = 1;      // Temperature Indicator is enabled

    ADPCHbits.PCH = 0b111100;  // GND channel selected
    ADCON1bits.DSEN = 1;       // double sample
    ADCON0bits.FM = 1;         // right justify
    ADCON0bits.CS = 1;         // Frc Clock
    ADCON0bits.ADON = 1;       // enable ADC module
}

uint16_t adc_getChannel(uint8_t channel) {
    ADPCHbits.PCH = channel;
    __delay_us(25);

    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);

    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);

    ADPCHbits.PCH = 0b111100;  // GND channel

    return (uint16_t)((ADRES + ADPREV) >> 1);  // return average of 2 measurements
}

uint16_t adc_getChannelViaVdd(uint8_t channel) {
    ADREFbits.PREF = 0b00;     // Vref+ is connected to Vdd
    ADPCHbits.PCH = channel;
    __delay_us(25);

    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);

    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);

    ADREFbits.PREF = 0b11;     // Vref+ is connected to internal Fixed Voltage Reference (FVR) module
    ADPCHbits.PCH = 0b111100;  // GND channel

    return (uint16_t)((ADRES + ADPREV) >> 1);  // return average of 2 measurements
}

void adc_measureBasic(uint16_t* voltage1, uint16_t* current1, uint16_t* voltage2, uint16_t* current2, uint16_t* voltage3, uint16_t* current3, uint16_t* voltage4, uint16_t* current4, uint16_t* voltage5, uint16_t* current5, uint16_t* temperature) {
    // all values are in mV, mA, 10th of a C

    *current1 = adc_getChannel(0b000000) * ADC_CURRENT_OUT_MULTIPLIER;   // ANA0
    *voltage1 = adc_getChannel(0b000001) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA1

    *current2 = adc_getChannel(0b000010) * ADC_CURRENT_OUT_MULTIPLIER;   // ANA2
    *voltage2 = adc_getChannel(0b000011) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA3

    *current3 = adc_getChannel(0b000100) * ADC_CURRENT_OUT_MULTIPLIER;   // ANA4
    *voltage3 = adc_getChannel(0b000101) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA5

    *current4 = adc_getChannel(0b000111) * ADC_CURRENT_OUT_MULTIPLIER;   // ANA7
    *voltage4 = adc_getChannel(0b000110) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA6

    *current5 = adc_getChannel(0b010000) * ADC_CURRENT_OUT_MULTIPLIER;   // ANC0
    *voltage5 = adc_getChannel(0b010001) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANC1

    *temperature = (adc_getChannel(0b010010) - ADC_TEMPERATURE_0C) / ADC_TEMPERATURE_BITS_PER_10THC;  // ANC2
}

void adc_measureExtra(uint16_t* voltage12V, uint16_t* voltage5V, uint16_t* temperatureDie) {
    *voltage12V = adc_getChannel(0b010011) * ADC_VOLTAGE_12V_MULTIPLIER;  // ANC3

    uint16_t fvrAdc = adc_getChannelViaVdd(0b111111);  // Fixed Voltage Reference
    *voltage5V = (uint16_t)(((uint32_t)4096 * 4096) / fvrAdc);

    uint16_t tempAdc = adc_getChannel(0b111101);  // Temperature Indicator
    *temperatureDie = (uint16_t)(900 + ((int32_t)tempAdc - (int32_t)DiaTSHR2) * DiaFVRA2X / (4095 * -(int32_t)3684 / 10000));
}
