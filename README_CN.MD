<div align="center" markdown="1">
  <img src=".github/LilyGo_logo.png" alt="LilyGo logo" width="100"/>
</div>

<h1 align="center">T-QUnit</h1>

## **[English](README.MD) | 中文**

## 最新消息

- 本仓库提供一个用于测试 T-QUnit 的 PAW-A350 最小寄存器读取示例。
- 测试结果通过 `115200` 波特率的串口输出。

## 1️⃣ 示例预览

```text
examples
└─PAW350RegisterRead                 # Product ID 与运动寄存器测试
```

## 2️⃣ 快速开始

示例默认使用所选 Arduino 开发板变体定义的 `SDA` 和 `SCL` 引脚。如需使用其他引脚，可修改 `PAW350RegisterRead.ino` 开头的 `T_QUNIT_I2C_SDA` 和 `T_QUNIT_I2C_SCL`。

> [!TIP]
> 示例使用ESP32,以下使用步骤以ESP32 Dev Module 作为演示,如果使用其他类型开发板请更改为对应的开发板选型
>

#### PlatformIO

1. 安装 [Visual Studio Code](https://code.visualstudio.com/) 和 [Python](https://www.python.org/)。
2. 在 Visual Studio Code 扩展中安装 `PlatformIO IDE`。
3. 扩展安装完成后重启 Visual Studio Code。
4. 选择 `File` -> `Open Folder`，打开 `T-QUnit` 目录。
5. 默认环境使用 ESP32 Dev Module：

```ini
board = esp32dev
```

6. 点击状态栏中的对勾进行编译。
7. 使用 USB 将主控开发板连接到电脑，然后点击上传箭头。
8. 打开串口监视器，波特率设置为 `115200`。

命令行编译：

```bash
pio run -e t_qunit
```

#### Arduino IDE

1. 安装 [Arduino IDE](https://www.arduino.cc/en/software)。
2. 安装主控开发板所需的 Arduino 开发板包，版本：` `。
3. 打开 `examples/PAW350RegisterRead/PAW350RegisterRead.ino`。
4. 在 `Tools` -> `Board` 中选择 `ESP32 Dev Module`
5. 其余 `Tools` 设置保持开发板默认值。
6. 连接主控开发板并点击 Upload。

启动后预期输出：

```text
T-QUnit PAW-A350 register read test
I2C: SDA=..., SCL=..., address=0x33
Product ID:  0x88 (expected 0x88) [PASS]
Revision ID: 0x..
Move a finger over the sensor to read motion deltas.
```

在传感器上移动手指时，串口会输出带符号的 X/Y 增量和累计值。

## 3️⃣ 注意事项

1. T-QUnit 上 PAW-A350 的 I2C 地址为 `0x33`。
2. 设备正常时，从 `0x00` 寄存器读取的 Product ID 应为 `0x88`。
3. 示例只读取事件寄存器和 X/Y 增量寄存器，不复位或配置传感器。
4. `DELTA_X` 和 `DELTA_Y` 是 8 位二进制补码有符号数。
5. Product ID 测试失败时，请检查供电、SDA/SCL 引脚、接线和 I2C 上拉电阻。

### 示例使用的 PAW-A350 寄存器

| 地址 | 名称 | 访问方式 | 说明 |
| ---- | ---- | -------- | ---- |
| `0x00` | PRODUCT_ID | 只读 | 预期值为 `0x88` |
| `0x01` | REVISION_ID | 只读 | 芯片修订版本 |
| `0x02` | EVENT | 读取 | Bit 7 表示检测到运动 |
| `0x03` | DELTA_X | 只读 | X 轴有符号运动增量 |
| `0x04` | DELTA_Y | 只读 | Y 轴有符号运动增量 |

