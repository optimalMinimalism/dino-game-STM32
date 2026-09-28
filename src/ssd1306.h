#ifndef SSD1306_H
#define SSD1306_H

#include "mbed.h"
#include "gameAssets.h"

// Indirizzo I2C 8-bit standard per SSD1306 in Mbed OS (0x3C << 1 = 0x78)
#define SSD1306_I2C_ADDR (0x3C << 1)

class Ssd1306 {
private:
    mbed::I2C &_i2c;
    uint8_t _frameBuffer[8][128]; // Buffer RAM (8 pagine x 128 colonne = 128x64 pixel)

    void sendCommand(uint8_t cmd);

public:
    explicit Ssd1306(mbed::I2C &i2cBus);
    
    void init();
    void clearBuffer();
    void updateDisplay();

    // --- METODI DI RENDERING SCHERMATE (Sfruttano gameAssets) ---
    void drawSplashScreen();
    void drawGameScene(int16_t dinoY, int16_t obstacleX, uint32_t score, uint32_t highScore);
    void drawGameOverScreen(uint32_t finalScore);
};

#endif // SSD1306_H