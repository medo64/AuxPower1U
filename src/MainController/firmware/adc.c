#include <xc.h>

#define ADC_VOLTAGE_OUT_MULTIPLIER       15  // 30 / 2
#define ADC_VOLTAGE_IN_MULTIPLIER     7 / 2  // 7 / 2
#define ADC_TEMPERATURE_0C              800  // 400mV / 2048mV * 4096
#define ADC_TEMPERATURE_BITS_PER_10THC    5  // 2048mV / 4096 * 10

void adc_init(void) {
    TRISA  |= 0b11111111;
    ANSELA |= 0b11111111;
    TRISC  |= 0b00001111;
    ANSELC |= 0b00001111;

    ADPCHbits.PCH = 0b111100;  // GND channel selected
    ADCON1bits.DSEN = 1;       // double sample
    ADCON0bits.FM = 1;         // right justify
    ADCON0bits.CS = 1;         // Frc Clock
    ADCON0bits.ADON = 1;       // enable ADC module

    // FVR @ 2.048
    FVRCONbits.ADFVR = 0b10;  // FVR Buffer 1 Gain is 2x, (2.048V)
    FVRCONbits.EN = 1;        // enable FVR
    while (!FVRCONbits.RDY);  // wait for ready
    ADREFbits.PREF = 0b11;  // Vref+ is connected to internal Fixed Voltage Reference (FVR) module
}

uint16_t adc_getChannel(uint8_t channel) {
    ADPCHbits.PCH = channel;

    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);

    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);

    ADPCHbits.PCH = 0b111100;  // GND channel

    return (uint16_t)((ADRES + ADPREV) >> 1);  // return average of 2 measurements
}

void adc_measureAll(uint16_t* voltage1, uint16_t* current1, uint16_t* voltage2, uint16_t* current2, uint16_t* voltage3, uint16_t* current3, uint16_t* voltage4, uint16_t* current4, uint16_t* voltage5, uint16_t* current5, uint16_t* temperature, uint16_t* voltage1in) {
    // all values are in mV, mA, 10th of a C

    *current1 = adc_getChannel(0b000000);                                // ANA0
    *voltage1 = adc_getChannel(0b000001) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA1

    *current2 = adc_getChannel(0b000010);                                // ANA2
    *voltage2 = adc_getChannel(0b000011) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA3

    *current3 = adc_getChannel(0b000100);                                // ANA4
    *voltage3 = adc_getChannel(0b000101) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA5

    *current4 = adc_getChannel(0b000111);                                // ANA7
    *voltage4 = adc_getChannel(0b000110) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANA6

    *current5 = adc_getChannel(0b010000);                                // ANC0
    *voltage5 = adc_getChannel(0b010001) * ADC_VOLTAGE_OUT_MULTIPLIER;   // ANC1

    *temperature = (adc_getChannel(0b010010) - ADC_TEMPERATURE_0C) / ADC_TEMPERATURE_BITS_PER_10THC;  // ANC2

    *voltage1in = adc_getChannel(0b010011) * ADC_VOLTAGE_IN_MULTIPLIER;  // ANC3
}
