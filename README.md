# Smart Automotive Ambient Lighting (STM32 + PySide6)

An interior lighting prototype that adapts a WS2812B LED strip to its environment and to driving dynamics. An STM32F103 reads temperature/humidity, motion and proximity sensors and drives the LEDs; a PySide6 desktop app configures everything over a small custom UART protocol.

Course project, Shenzhen Technology University, Sep – Dec 2025.


https://github.com/user-attachments/assets/0c00677c-3ed4-4f04-b851-fef87b816fa6



## Features

**Smart mode** – each function can be toggled independently from the GUI:

| Function | Sensor | Behaviour |
|---|---|---|
| Temperature adaptation | DHT11 | Shifts colour warmer (R+40, B−40) below the low threshold and cooler (R−40, B+40) above the high threshold, with a 1 s gradient between states |
| Humidity adaptation | DHT11 | Dims the strip to 20 % above the humidity threshold to reduce glare in fog-like conditions |
| Welcome light | TCRT5000 (IR) | Plays a fade-in and centre-out sweep animation (twice) after a person is detected for more than 5 s |
| Driving response | MPU6050 | Left / right turn and straight-line acceleration each trigger a directional flowing animation |
| Preset scenes | – | Default, Long trip (slow breathing), Ambient (flowing), Emergency (red flashing) |

**Custom mode** – pick any RGB colour (sliders or colour dialog) and one of four effects: steady, breathing, blinking, flowing.

Thresholds are set in `firmware/User/main.c`. The committed values are the ones used for the bench demo (60 % RH; 30 °C / 32 °C) so the effects can be triggered by hand.

An OLED shows live temperature, humidity, IR reading and motion values.

## System overview

```
PySide6 GUI  ──UART 115200──►  STM32F103C8  ──►  WS2812B strip
                                   ▲
                 DHT11 · MPU6050 · TCRT5000 · OLED
```

### Serial protocol

ASCII frames: `#CMD,param1,param2*` (`#` = start, `*` = end)

| Command | Parameters | Meaning |
|---|---|---|
| `MODE` | `1` / `0` | Smart mode / custom mode |
| `TEMP`, `HUMI`, `WELCOME`, `DRIVE` | `1` / `0` | Enable / disable a smart function |
| `SCENE` | `1`–`4` | Default, long trip, ambient, emergency |
| `LIGHT` | `1`–`4` | Steady, breathing, blinking, flowing |
| `COLOR` | `r,g,b` (0–255) | Set base colour |

Frames are received byte-by-byte in the USART1 interrupt and parsed in the main loop. The firmware echoes each accepted command (e.g. `MODE:1`), which the GUI displays.

## Hardware

| Component | Interface | Pins |
|---|---|---|
| USB–TTL adapter (PC link) | USART1 | PA9 (TX), PA10 (RX) |
| WS2812B strip (5 LEDs, separate 5 V supply) | SPI1 MOSI + DMA1 channel 3 | PA7 |
| DHT11 | Single-wire | PA1 |
| TCRT5000 | ADC1 CH0 + DMA | PA0 |
| MPU6050 | Software I²C | PB10, PB11 |
| OLED | Software I²C | PB8 (SCL), PB9 (SDA) |

Each WS2812 bit is encoded as one SPI byte (`0xF8` = 1, `0xE0` = 0), and DMA streams the whole frame so the timing does not depend on the CPU.

## Repository layout

```
firmware/        Keil uVision5 project (STM32F10x Standard Peripheral Library V3.5)
  User/          main.c – initialisation, main loop, command parser
  Hardware/      Drivers: WS2812, DHT11, MPU6050, OLED, Serial, ADC
  System/        Delay, timer, MPU6050 filter
  Library/ Start/  ST peripheral library and startup code
host/            PySide6 desktop application
```

## Getting started

**Firmware:** open `firmware/project.uvprojx` in Keil uVision5, build, and flash to an STM32F103C8.

**Desktop app** (developed with Python 3.13):

```bash
cd host
pip install -r requirements.txt
python main.py
```

Select the serial port, click connect, then choose a mode. The UI labels are in Chinese.

## Engineering notes

- **Non-blocking effects.** Early animations used `Delay` loops, which froze the firmware so colour commands were missed. Effects in custom mode were rewritten as a state machine (`ws2812_effect_update`) advanced by a counter, so commands are handled while an animation runs.
- **One-shot dimming.** The humidity dimming was re-triggered on every loop iteration, making brightness ramp down repeatedly. A state flag now ensures the gradient runs once per threshold crossing.
- **MPU6050 noise.** Raw gyro/accelerometer data caused false turn triggers at rest. Fixed with a 20-sample zero-offset calibration at start-up plus a first-order low-pass and moving-average filter.

## Limitations

- Bench prototype with a 5-LED strip; thresholds were tuned on the desk, not in a vehicle.
- The driving, welcome and colour-gradient animations still use blocking delays, so commands sent while they play are handled afterwards.

## Author

Qitong Yuan – [github.com/qitong-yuan](https://github.com/qitong-yuan)
