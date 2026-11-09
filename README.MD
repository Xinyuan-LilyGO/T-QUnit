<div align="center" markdown="1">
  <img src=".github/LilyGo_logo.png" alt="LilyGo logo" width="100"/>
</div>

<h1 align="center">T-QUnit</h1>

## **English | [中文](README_CN.MD)**

## News

- The repository contains a minimal PAW-A350 register-read example for T-QUnit.
- Test results are printed through the serial port at `115200` baud.

## 1️⃣ Examples preview

```text
examples
└─PAW350RegisterRead                 # Product ID and motion register test
```

## 2️⃣ Quick Start

The example uses the `SDA` and `SCL` pins defined by the selected Arduino board variant. To use different pins, change `T_QUNIT_I2C_SDA` and `T_QUNIT_I2C_SCL` near the top of `PAW350RegisterRead.ino`.

> [!TIP]
> This example uses an ESP32. The following steps are demonstrated using an "ESP32 Dev Module"; if you are using a different type of development board, please select the appropriate board model instead.
>

#### PlatformIO

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/).
2. Install the `PlatformIO IDE` extension in Visual Studio Code.
3. Restart Visual Studio Code after the extension installation finishes.
4. Select `File` -> `Open Folder`, then open the `T-QUnit` directory.
5. The default environment uses the ESP32 Dev Module:

```ini
board = esp32dev
```

6. Click the check mark in the status bar to compile.
7. Connect the host board to the computer and click the upload arrow.
8. Open the serial monitor at `115200` baud.

Command-line build:

```bash
pio run -e t_qunit
```

#### Arduino IDE

1. Install [Arduino IDE](https://www.arduino.cc/en/software).
2. Install the Arduino board package required by the host board. Version: ` `.
3. Open `examples/PAW350RegisterRead/PAW350RegisterRead.ino`.
4. Select `ESP32 Dev Module` under `Tools` -> `Board`.
5. Keep the remaining `Tools` settings at their board defaults.
6. Connect the host board and click Upload.

Expected startup output:

```text
T-QUnit PAW-A350 register read test
I2C: SDA=..., SCL=..., address=0x33
Product ID:  0x88 (expected 0x88) [PASS]
Revision ID: 0x..
Move a finger over the sensor to read motion deltas.
```

Moving a finger over the sensor prints signed X/Y deltas and their accumulated values.

## 3️⃣ Tips

1. T-QUnit uses the PAW-A350 I2C address `0x33`.
2. A working device returns product ID `0x88` from register `0x00`.
3. The example reads the event register and the X/Y delta registers only. It does not reset or configure the sensor.
4. `DELTA_X` and `DELTA_Y` are signed 8-bit two's-complement values.
5. If the product ID test fails, check the power supply, SDA/SCL selection, wiring, and I2C pull-up resistors.

### PAW-A350 registers used by the example

| Address | Name | Access | Description |
| ------- | ---- | ------ | ----------- |
| `0x00` | PRODUCT_ID | Read only | Expected value: `0x88` |
| `0x01` | REVISION_ID | Read only | Silicon revision |
| `0x02` | EVENT | Read | Bit 7 indicates motion |
| `0x03` | DELTA_X | Read only | Signed X movement |
| `0x04` | DELTA_Y | Read only | Signed Y movement |

