/**
 * @file      PAW350RegisterRead.ino
 * @license   MIT
 * @copyright Copyright (c) 2026 LilyGO
 * @brief     Read the PAW-A350 registers on T-QUnit
 */

#include <Arduino.h>
#include <Wire.h>

#ifndef T_QUNIT_I2C_SDA
#define T_QUNIT_I2C_SDA       SDA
#endif

#ifndef T_QUNIT_I2C_SCL
#define T_QUNIT_I2C_SCL       SCL
#endif

#ifndef T_QUNIT_I2C_FREQUENCY
#define T_QUNIT_I2C_FREQUENCY 100000U
#endif

namespace
{
constexpr uint8_t PAW_A350_ADDRESS = 0x33;
constexpr uint8_t PAW_A350_PRODUCT_ID = 0x88;

constexpr uint8_t REG_PRODUCT_ID = 0x00;
constexpr uint8_t REG_REVISION_ID = 0x01;
constexpr uint8_t REG_EVENT = 0x02;
constexpr uint8_t REG_DELTA_X = 0x03;
constexpr uint8_t EVENT_MOTION = 0x80;

bool readRegisters(uint8_t reg, uint8_t *data, size_t length)
{
    Wire.beginTransmission(PAW_A350_ADDRESS);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        return false;
    }

    const size_t received = Wire.requestFrom(
        PAW_A350_ADDRESS, static_cast<uint8_t>(length));
    if (received != length) {
        while (Wire.available()) {
            Wire.read();
        }
        return false;
    }

    for (size_t i = 0; i < length; ++i) {
        data[i] = static_cast<uint8_t>(Wire.read());
    }
    return true;
}

bool readRegister(uint8_t reg, uint8_t &value)
{
    return readRegisters(reg, &value, 1);
}
} // namespace

void setup()
{
    Serial.begin(115200);
    delay(200);

    Wire.begin(T_QUNIT_I2C_SDA, T_QUNIT_I2C_SCL);
    Wire.setClock(T_QUNIT_I2C_FREQUENCY);

    Serial.println();
    Serial.println("T-QUnit PAW-A350 register read test");
    Serial.printf("I2C: SDA=%d, SCL=%d, address=0x%02X\n",
                  T_QUNIT_I2C_SDA, T_QUNIT_I2C_SCL, PAW_A350_ADDRESS);

    uint8_t productId = 0;
    uint8_t revisionId = 0;
    if (!readRegister(REG_PRODUCT_ID, productId) ||
            !readRegister(REG_REVISION_ID, revisionId)) {
        Serial.println("[FAIL] PAW-A350 did not respond. Check power and I2C wiring.");
        return;
    }

    Serial.printf("Product ID:  0x%02X (expected 0x%02X) %s\n",
                  productId, PAW_A350_PRODUCT_ID,
                  productId == PAW_A350_PRODUCT_ID ? "[PASS]" : "[FAIL]");
    Serial.printf("Revision ID: 0x%02X\n", revisionId);
    Serial.println("Move a finger over the sensor to read motion deltas.");
}

void loop()
{
    static int32_t totalX = 0;
    static int32_t totalY = 0;
    static uint32_t lastErrorMillis = 0;

    uint8_t event = 0;
    if (!readRegister(REG_EVENT, event)) {
        if (millis() - lastErrorMillis >= 1000) {
            Serial.println("[FAIL] I2C register read failed");
            lastErrorMillis = millis();
        }
        delay(10);
        return;
    }

    if ((event & EVENT_MOTION) != 0) {
        uint8_t rawDelta[2] = {0, 0};
        if (readRegisters(REG_DELTA_X, rawDelta, sizeof(rawDelta))) {
            const int8_t deltaX = static_cast<int8_t>(rawDelta[0]);
            const int8_t deltaY = static_cast<int8_t>(rawDelta[1]);
            totalX += deltaX;
            totalY += deltaY;

            Serial.printf("DX:%4d DY:%4d TotalX:%7ld TotalY:%7ld\n",
                          deltaX, deltaY,
                          static_cast<long>(totalX),
                          static_cast<long>(totalY));
        }
    }

    delay(10);
}

