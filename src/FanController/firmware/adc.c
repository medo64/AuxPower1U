#include <xc.h>
#include <stdint.h>

#define ADC_VALUE_0C    82
#define ADC_BITS_PER_C   4   // 19.5mV/C / (5000mV/1024)

uint8_t adc_get_temperature(void) {
    ADCON0bits.GO_nDONE = 1;      // start an A/D conversion cycle
    while (ADCON0bits.GO_nDONE);  // wait for conversion to complete
    uint16_t value1 = ADRES;

    ADCON0bits.GO_nDONE = 1;      // start an A/D conversion cycle
    while (ADCON0bits.GO_nDONE);  // wait for conversion to complete
    uint16_t value2 = ADRES;

    uint16_t valueAvg = (value1 + value2 + 1) / 2;

    if (valueAvg <= ADC_VALUE_0C) { return 0; }  // don't measure negative values

    return (uint8_t)((valueAvg - ADC_VALUE_0C) / ADC_BITS_PER_C);
}


void adc_init(void) {
    // Pin Setup
    TRISA  |= 0b000010;      // PWM4 PWM3 MCLR PWM1 Temp PWM2
    ANSELA |= 0b000010;
    WPUA   &= 0b111101;

    // ADC setup
    ADCON1bits.ADCS = 0b111;   // Select ADC conversion clock [FRC]
    ADCON1bits.ADPREF = 0b00;  // Configure voltage reference [VRPOS is connected to VDD]
    ADCON1bits.ADFM = 1;       // right justified
    ADCON0bits.CHS = 0b00001;  // Channel selection [AN1]
    ADCON0bits.ADON = 1;       // ADC is enabled
}
