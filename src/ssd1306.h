#ifndef SSD1306_H
#define SSD1306_H

#include "mbed.h"

#define SSD1306_I2C_ADDR (0x3C << 1)

class Ssd1306 {
private:
    mbed::I2C &_i2c;
    uint8_t _frameBuffer[8][128];

    void sendCommand(uint8_t cmd);
    void setPixel(int16_t x, int16_t y);

public:
    explicit Ssd1306(mbed::I2C &i2cBus);
    
    void init();
    void clearBuffer();
    void updateDisplay();
    void drawSkullAndBones(); // Tutto in una sola funzione pulita!
};

#endif // SSD1306_H