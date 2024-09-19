## FanController

|      |                           |
|------|---------------------------|
| Size | 24.0 x 36.0 mm ~ 1.4 sqin |


### Parts

|  # | Part name                                    | RefDes  | Source                                                                            |
|---:|----------------------------------------------|---------|-----------------------------------------------------------------------------------|
|  2 | C 100 nF (0805)                              | C1-C2   |                                                                                   |
|  1 | C 1 uF (0805)                                | C3      |                                                                                   |
|  1 | J MC 1,5/ 2-G-3,81                           | J1      | DigiKey: 277-1206-ND                                                              |
|  4 | J Fan PWM (4w)                               | J2-J5   | DigiKey: WM4330-ND                                                                |
|  1 | U Micro [PIC12F1501] (SOIC-8)                | U1      | DigiKey: PIC12F1501-I/SN-ND                                                       |
|  1 | U Temperature Analog [MCP9701A] (SOT23-3)    | U2      | DigiKey: MCP9701AT-E/TTCT-ND                                                      |
|  1 | VR Linear 5V 100mA [AS78L05RTR-E1] (SOT89-3) | VR1     | DigiKey: AS78L05RTR-E1DICT-ND                                                     |


### Pin Usage

|  # | Pin | ICSP | Function | Description        |
|---:|:---:|------|----------|--------------------|
|  1 | Vdd | Vdd  |          |                    |
|  2 | RA5 |      | PWM4     | Fan 4 Control      |
|  3 | RA4 |      | PWM3     | Fan 3 Control      |
|  4 | RA3 | Vpp  |          |                    |
|  5 | RA2 |      | PWM1     | Fan 1 Control      |
|  6 | RA1 | Clk  | AN1      | Temperature sensor |
|  7 | RA0 | Dat  | PWM2     | Fan 2 Control      |
|  8 | Vss | Vss  |          |                    |
