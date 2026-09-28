#include "ssd1306.h"
#include <cstring>

// Bitmap 16x16 del Teschio
static const uint16_t skullRows[16] = {
    0x07E0, 0x1FF8, 0x3FFC, 0x7FFE,
    0x7FFE, 0x6DB6, 0x7FFE, 0x3FFC,
    0x1FF8, 0x0FF0, 0x0660, 0x0FF0,
    0x0A50, 0x0A50, 0x0FF0, 0x0000
};

Ssd1306::Ssd1306(mbed::I2C &i2cBus) : _i2c(i2cBus) {
    clearBuffer();
}

void Ssd1306::sendCommand(uint8_t cmd) {
    char data[2] = {0x00, static_cast<char>(cmd)};
    _i2c.write(SSD1306_I2C_ADDR, data, 2);
}

void Ssd1306::setPixel(int16_t x, int16_t y) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;
    _frameBuffer[y / 8][x] |= (1 << (y % 8));
}

void Ssd1306::clearBuffer() {
    memset(_frameBuffer, 0, sizeof(_frameBuffer));
}

void Ssd1306::init() {
    _i2c.frequency(400000); // 400kHz Fast I2C mode
    ThisThread::sleep_for(50ms);

    sendCommand(0xAE); // Display OFF
    sendCommand(0xD5); sendCommand(0x80); // Set Clock Ratio
    sendCommand(0xA8); sendCommand(0x3F); // Set Multiplex Ratio (1/64)
    sendCommand(0xD3); sendCommand(0x00); // Display Offset 0
    sendCommand(0x40); // Start Line 0
    sendCommand(0x8D); sendCommand(0x14); // Enable Charge Pump
    sendCommand(0x20); sendCommand(0x00); // Horizontal Addressing Mode
    sendCommand(0xA1); // Segment Re-map
    sendCommand(0xC8); // COM Output Scan Direction
    sendCommand(0xDA); sendCommand(0x12); // Hardware Pin Config
    sendCommand(0x81); sendCommand(0xCF); // Contrast
    sendCommand(0xD9); sendCommand(0xF1); // Pre-charge
    sendCommand(0xDB); sendCommand(0x40); // VCOMH
    sendCommand(0xA4); // Resume to RAM content
    sendCommand(0xA6); // Normal Display
    sendCommand(0xAF); // Display ON
}

// ============================================================================
// SKULL AND BONES DRAW FUNCTION
// ============================================================================
void Ssd1306::drawSkullAndBones() {
    clearBuffer();

    // 1. Disegno delle 2 Tibie incrociate direttamente con i 4 estremi
    const int16_t bones[2][4] = {
        {38, 52, 90, 26}, // Tibia 1
        {38, 26, 90, 52}  // Tibia 2
    };

    for (uint8_t b = 0; b < 2; ++b) {
        int16_t x0 = bones[b][0], y0 = bones[b][1];
        int16_t x1 = bones[b][2], y1 = bones[b][3];

        // Aste delle tibie
        for (int16_t step = 0; step <= 52; ++step) {
            int16_t x = x0 + (x1 - x0) * step / 52;
            int16_t y = y0 + (y1 - y0) * step / 52;
            for (int16_t offset = -1; offset <= 1; ++offset) {
                setPixel(x + offset, y);
                setPixel(x, y + offset);
            }
        }

        // Pomelli/Cerchi alle 4 estremita' delle tibie
        const int16_t ends[2][2] = {{x0, y0}, {x1, y1}};
        for (uint8_t end = 0; end < 2; ++end) {
            for (int16_t dy = -3; dy <= 3; ++dy) {
                for (int16_t dx = -3; dx <= 3; ++dx) {
                    if (dx * dx + dy * dy <= 9) {
                        setPixel(ends[end][0] + dx, ends[end][1] + dy);
                    }
                }
            }
        }
    }

    // 2. Disegno del Teschio (16x16 scalato 2x a centro schermo)
    for (uint8_t sourceY = 0; sourceY < 16; ++sourceY) {
        for (uint8_t sourceX = 0; sourceX < 16; ++sourceX) {
            if ((skullRows[sourceY] & (1U << (15U - sourceX))) == 0U) {
                continue;
            }

            for (uint8_t dy = 0; dy < 2; ++dy) {
                for (uint8_t dx = 0; dx < 2; ++dx) {
                    setPixel(48 + sourceX * 2 + dx, 8 + sourceY * 2 + dy);
                }
            }
        }
    }

    updateDisplay();
}

void Ssd1306::updateDisplay() {
    for (uint8_t page = 0; page < 8; ++page) {
        sendCommand(0xB0 + page); // Imposta indirizzo pagina
        sendCommand(0x00);        // Lower Column
        sendCommand(0x10);        // Higher Column

        char pageData[129];
        pageData[0] = 0x40; // Data Prefix
        memcpy(&pageData[1], _frameBuffer[page], 128);

        _i2c.write(SSD1306_I2C_ADDR, pageData, 129);
    }
}