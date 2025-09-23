#include <stdbool.h>
#include <stdint.h>

#include "adc.h"

#define AVG_SHIFT  3

#define CUTOFF_POWER_VA  94500000  //  90W +5%

uint16_t CutoffCurrent1;
uint16_t CutoffCurrent2;
uint16_t CutoffCurrent3;
uint16_t CutoffCurrent4;
uint16_t CutoffCurrent5;

uint32_t Voltage1Sum, Voltage2Sum, Voltage3Sum, Voltage4Sum, Voltage5Sum;
uint32_t Current1Sum, Current2Sum, Current3Sum, Current4Sum, Current5Sum;
uint32_t TemperatureSum;

void measurement_init(uint16_t cutoffCurrent1, uint16_t cutoffCurrent2, uint16_t cutoffCurrent3, uint16_t cutoffCurrent4, uint16_t cutoffCurrent5) {
    // fast cutoff point
    CutoffCurrent1 = cutoffCurrent1;
    CutoffCurrent2 = cutoffCurrent2;
    CutoffCurrent3 = cutoffCurrent3;
    CutoffCurrent4 = cutoffCurrent4;
    CutoffCurrent5 = cutoffCurrent5;

    // ADC setup
    adc_init();
    uint16_t voltage1, current1, voltage2, current2, voltage3, current3, voltage4, current4, voltage5, current5, temperature;
    adc_measureBasic(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature);

    // lower current measurement to avoid tripping average current protection due to a single measurement
    if (current1 > 100) { current1 -= 100; } else { current1 = 0; }
    if (current2 > 100) { current2 -= 100; } else { current2 = 0; }
    if (current3 > 100) { current3 -= 100; } else { current3 = 0; }
    if (current4 > 100) { current4 -= 100; } else { current4 = 0; }
    if (current5 > 100) { current5 -= 100; } else { current5 = 0; }

    // expand initial measurement
    Voltage1Sum = (uint32_t)voltage1 << AVG_SHIFT, Current1Sum = (uint32_t)current1 << AVG_SHIFT;
    Voltage2Sum = (uint32_t)voltage2 << AVG_SHIFT, Current2Sum = (uint32_t)current2 << AVG_SHIFT;
    Voltage3Sum = (uint32_t)voltage3 << AVG_SHIFT, Current3Sum = (uint32_t)current3 << AVG_SHIFT;
    Voltage4Sum = (uint32_t)voltage4 << AVG_SHIFT, Current4Sum = (uint32_t)current4 << AVG_SHIFT;
    Voltage5Sum = (uint32_t)voltage5 << AVG_SHIFT, Current5Sum = (uint32_t)current5 << AVG_SHIFT;
    TemperatureSum = (uint32_t)temperature << AVG_SHIFT;

}

void measurement_basic(uint8_t* nextOutputs, uint16_t* voltage1, uint16_t* current1, uint16_t* voltage2, uint16_t* current2, uint16_t* voltage3, uint16_t* current3, uint16_t* voltage4, uint16_t* current4, uint16_t* voltage5, uint16_t* current5, uint16_t* temperature) {
    uint16_t u1, i1, u2, i2, u3, i3, u4, i4, u5, i5, t;
    adc_measureBasic(&u1, &i1, &u2, &i2, &u3, &i3, &u4, &i4, &u5, &i5, &t);

    // cutoff if exceeding current
    if (i1 > CutoffCurrent1) { *nextOutputs &= 0b11110; }
    if (i2 > CutoffCurrent2) { *nextOutputs &= 0b11101; }
    if (i3 > CutoffCurrent3) { *nextOutputs &= 0b11011; }
    if (i4 > CutoffCurrent4) { *nextOutputs &= 0b10111; }
    if (i5 > CutoffCurrent5) { *nextOutputs &= 0b01111; }

    // cutoff if exceeding power
    if (((uint32_t)u1 * i1) > CUTOFF_POWER_VA) { *nextOutputs &= 0b11110; }
    if (((uint32_t)u2 * i2) > CUTOFF_POWER_VA) { *nextOutputs &= 0b11101; }
    if (((uint32_t)u3 * i3) > CUTOFF_POWER_VA) { *nextOutputs &= 0b11011; }
    if (((uint32_t)u4 * i4) > CUTOFF_POWER_VA) { *nextOutputs &= 0b10111; }
    if (((uint32_t)u5 * i5) > CUTOFF_POWER_VA) { *nextOutputs &= 0b01111; }

    // calculate average from previous tick
    uint16_t voltage1Avg = (uint16_t)(Voltage1Sum >> AVG_SHIFT);
    uint16_t voltage2Avg = (uint16_t)(Voltage2Sum >> AVG_SHIFT);
    uint16_t voltage3Avg = (uint16_t)(Voltage3Sum >> AVG_SHIFT);
    uint16_t voltage4Avg = (uint16_t)(Voltage4Sum >> AVG_SHIFT);
    uint16_t voltage5Avg = (uint16_t)(Voltage5Sum >> AVG_SHIFT);
    uint16_t current1Avg = (uint16_t)(Current1Sum >> AVG_SHIFT);
    uint16_t current2Avg = (uint16_t)(Current2Sum >> AVG_SHIFT);
    uint16_t current3Avg = (uint16_t)(Current3Sum >> AVG_SHIFT);
    uint16_t current4Avg = (uint16_t)(Current4Sum >> AVG_SHIFT);
    uint16_t current5Avg = (uint16_t)(Current5Sum >> AVG_SHIFT);
    uint16_t temperatureAvg = (uint16_t)(TemperatureSum >> AVG_SHIFT);

    // add new value to averages
    Voltage1Sum -= voltage1Avg; Voltage1Sum += u1;
    Voltage2Sum -= voltage2Avg; Voltage2Sum += u2;
    Voltage3Sum -= voltage3Avg; Voltage3Sum += u3;
    Voltage4Sum -= voltage4Avg; Voltage4Sum += u4;
    Voltage5Sum -= voltage5Avg; Voltage5Sum += u5;
    Current1Sum -= current1Avg; Current1Sum += i1;
    Current2Sum -= current2Avg; Current2Sum += i2;
    Current3Sum -= current3Avg; Current3Sum += i3;
    Current4Sum -= current4Avg; Current4Sum += i4;
    Current5Sum -= current5Avg; Current5Sum += i5;
    TemperatureSum -= temperatureAvg; TemperatureSum += t;

    *voltage1 = (uint16_t)(Voltage1Sum >> AVG_SHIFT);
    *current1 = (uint16_t)(Current1Sum >> AVG_SHIFT);
    *voltage2 = (uint16_t)(Voltage2Sum >> AVG_SHIFT);
    *current2 = (uint16_t)(Current2Sum >> AVG_SHIFT);
    *voltage3 = (uint16_t)(Voltage3Sum >> AVG_SHIFT);
    *current3 = (uint16_t)(Current3Sum >> AVG_SHIFT);
    *voltage4 = (uint16_t)(Voltage4Sum >> AVG_SHIFT);
    *current4 = (uint16_t)(Current4Sum >> AVG_SHIFT);
    *voltage5 = (uint16_t)(Voltage5Sum >> AVG_SHIFT);
    *current5 = (uint16_t)(Current5Sum >> AVG_SHIFT);
    *temperature = (uint16_t)(TemperatureSum >> AVG_SHIFT);
}

void measurement_extra(uint16_t* voltage12V, uint16_t* voltage5V, uint16_t* temperatureDie) {
    adc_measureExtra(voltage12V, voltage5V, temperatureDie);
}
