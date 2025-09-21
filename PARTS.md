## AuxPower PCB

### Properties

|      |                            |
|------|----------------------------|
| Size | 90.0 x 90.0 mm ~ 15.2 sqin |


### Parts

|  # | Part name                                          | RefDes   | Source                                                      |
|---:|----------------------------------------------------|----------|-------------------------------------------------------------|
|  5 | AT PhotoMOS 6A 60V [AQZ202G] (SIP-4)               | AT1-AT5  | 255-5699-ND                                                 |
|  5 | C 100nF X7R 50V (0805)                             | C1-c5    | 478-5311-1-ND                                               |
|  2 | C 1uF X7R 25V (0805)                               | C6-C7    | 1276-1066-1-ND                                              |
| 12 | DS LED Red (3mm)                                   | DS1-DS12 | 754-1218-ND                                                 |
|  5 | J Phoenix MC 1,5/ 2-G-3,81 R/A (2w)                | J1-J5    | ED2808-ND                                                   |
|  5 | J Phoenix MC 1,5/ 4-G-3,81 R/A (4w)                | J6-J10   | ED2810-ND                                                   |
|  5 | J JST XH r/a (3w)                                  | J11-J15  | 455-S3B-XH-A-ND                                             |
|  1 | J JST XH (3w)                                      | J16      | 455-2248-ND                                                 |
|  1 | J JST XH (4w)                                      | J17      | 455-B4B-XH-A-ND                                             |
|  5 | R 0.025 1% 3W (2512)                               | R1-R5    | CRA2512-FZ-R025ELFCT-ND                                     |
|  5 | R 470 1% 0.125W (0805)                             | R6-R10   | RMCF0805FT470RCT-ND                                         |
|  5 | R 1.5K 1% 0.125W (0805)                            | R11-R15  | RMCF0805FT1K50CT-ND                                         |
| 20 | R 3.3K 1% 0.125W (0805)                            | R16-R35  | RMCF0805FT3K30CT-ND                                         |
|  5 | R 6.19K 0.1% 0.125W (0805)                         | R36-R40  | P6.19KDACT-ND                                               |
|  1 | R 6.80K 0.1% 0.125W (0805)                         | R41      | P6.8KDACT-ND                                                |
|  1 | R 102K 0.1% 0.125W 1% (0805)                       | R42      | P102KDACT-ND                                                |
|  5 | R 390K 0.1% 0.125W (0805)                          | R43-R47  | P390KDACT-ND                                                |
|  5 | U CurrentMonitor [HV7801] (SOT23-5)                | U1-U5    | HV7801K1-GCT-ND                                             |
|  1 | U Micro [PIC18F26K83] (SOIC-28)                    | U6       | PIC18F26K83-I/SO-ND                                         |
|  1 | U Expander I2C [PI4IOE5V6416LEX] (TSSOP-24)        | U7       | PI4IOE5V6416LEXDICT-ND                                      |
|  1 | U Temperature Analog 2% [MCP9701A] (SOT23-3)       | U8       | MCP9701AT-E/TTCT-ND                                         |
|  1 | VR Linear 5V 1A [AS7805ADTR-G1] (TO252-2)          | VR1      | AS7805ADTR-G1DICT-ND                                        |
|  5 | P Phoenix MC 1,5/ 2-ST-3,81 (2w)                   | P1-P5    | ED2875-ND                                                   |
|  5 | P Phoenix MC 1,5/ 4-ST-3,81 (4w)                   | P6-P10   | ED2877-ND                                                   |


### Pin Usage (PIC18F26K83)

|  # | Pin | ICSP | Function | Description                                |
|---:|:---:|------|----------|--------------------------------------------|
|  1 | RE3 | MCLR |          |                                            |
|  2 | RA0 |      | ANA0     | Out 1: Current (ADC 1.024V 1:5)            |
|  3 | RA1 |      | ANA1     | Out 1: Voltage (ADC 1.024V 16mV/bit; 1:64) |
|  4 | RA2 |      | ANA2     | Out 2: Current (ADC 1.024V 1:5)            |
|  5 | RA3 |      | ANA3     | Out 2: Voltage (ADC 1.024V 16mV/bit; 1:64) |
|  6 | RA4 |      | ANA4     | Out 3: Current (ADC 1.024V 1:5)            |
|  7 | RA5 |      | ANA5     | Out 3: Voltage (ADC 1.024V 16mV/bit; 1:64) |
|  8 | Vss | GND  | -        | -                                          |
|  9 | RA7 |      | ANA7     | Out 4: Current (ADC 1.024V 1:5)            |
| 10 | RA6 |      | ANA6     | Out 4: Voltage (ADC 1.024V 16mV/bit; 1:64) |
| 11 | RC0 |      | ANC0     | Out 5: Current (ADC 1.024V 1:5)            |
| 12 | RC1 |      | ANC1     | Out 5: Voltage (ADC 1.024V 16mV/bit; 1:64) |
| 13 | RC2 |      | ANC2     | Temperature sensor (ADC 2.048V 0.02C/bit)  |
| 14 | RC3 |      | ANC3     | In 1: Voltage  (ADC 1.024V 4mV/bit; 1:16)  |
| 15 | RC4 |      | C4       | Out 5: Control                             |
| 16 | RC5 |      | C5       | Out 4: Control                             |
| 17 | RC6 |      | C6       | Out 3: Control                             |
| 18 | RC7 |      | C7       | Out 2: Control                             |
| 19 | Vss | GND  | -        | -                                          |
| 20 | Vdd | 5V   | -        | -                                          |
| 21 | RB0 |      | B0       | Out 1: Control                             |
| 22 | RB1 |      | I2C SCL  | I2C                                        |
| 23 | RB2 |      | I2C SDA  | I2C                                        |
| 24 | RB3 |      | B3       | LED: Activity                              |
| 25 | RB4 |      | UART TX  | UART                                       |
| 26 | RB5 |      | UART RX  | UART                                       |
| 27 | RB6 | CLK  |          |                                            |
| 28 | RB7 | DAT  |          |                                            |


### Pin Usage (PI4IOE5V6416)

|  # | Pin   | ICSP | Description        |
|---:|:-----:|------|--------------------|
|  1 | INT   |      | -                  |
|  2 | Vdd/I | 5V   | -                  |
|  3 | RESET |      | -                  |
|  4 | P0_0  |      | ~~Out 1: Control~~ |
|  5 | P0_1  |      | ~~Out 2: Control~~ |
|  6 | P0_2  |      | ~~Out 3: Control~~ |
|  7 | P0_3  |      | ~~Out 4: Control~~ |
|  8 | P0_4  |      | ~~Out 5: Control~~ |
|  9 | P0_5  |      |                    |
| 10 | P0_6  |      | Button 5: ~Switch  |
| 11 | P0_7  |      | Button 5: LED      |
| 12 | Vss   | GND  | -                  |
| 13 | P1_0  |      | Button 4: LED      |
| 14 | P1_1  |      | Button 4: ~Switch  |
| 15 | P1_2  |      | Button 3: LED      |
| 16 | P1_3  |      | Button 3: ~Switch  |
| 17 | P1_4  |      | Button 2: LED      |
| 18 | P1_5  |      | Button 2: ~Switch  |
| 19 | P1_6  |      | Button 1: LED      |
| 20 | P1_7  |      | Button 1: ~Switch  |
| 21 | Vss   | GND  | -                  |
| 22 | SCL   |      | I2C SCL            |
| 23 | SDA   |      | I2C SDA            |
| 24 | Vdd/P | 5V   | -                  |
