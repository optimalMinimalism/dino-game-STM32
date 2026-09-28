#include "ssd1306.h"
#include <cstring>
#include <cstdio>

Ssd1306::Ssd1306(mbed::I2C &i2cBus) : _i2c(i2cBus) {
    clearBuffer();
}

/**
 * Invia un comando singolo al controller SSD1306 via I2C
 */
void Ssd1306::sendCommand(uint8_t cmd) {
    char data[2] = {0x00, static_cast<char>(cmd)};
    _i2c.write(SSD1306_I2C_ADDR, data, 2);
}

/**
 * Resetta a zero il frame buffer in RAM
 */
void Ssd1306::clearBuffer() {
    memset(_frameBuffer, 0, sizeof(_frameBuffer));
}

/**
 * Sequenza di inizializzazione hardware dell'OLED SSD1306 128x64
 */
void Ssd1306::init() {
    _i2c.frequency(400000); // 400 kHz Fast-Mode I2C
    ThisThread::sleep_for(50ms);

    sendCommand(0xAE); // Display OFF
    sendCommand(0xD5); sendCommand(0x80); // Set Clock Divide Ratio
    sendCommand(0xA8); sendCommand(0x3F); // Set Multiplex Ratio (64 lines)
    sendCommand(0xD3); sendCommand(0x00); // Display Offset 0
    sendCommand(0x40);                     // Start Line 0
    sendCommand(0x8D); sendCommand(0x14); // Enable Charge Pump (3.3V supply)
    sendCommand(0x20); sendCommand(0x00); // Horizontal Addressing Mode
    sendCommand(0xA1);                     // Segment Re-map (flip orizzontale)
    sendCommand(0xC8);                     // COM Scan Direction (flip verticale)
    sendCommand(0xDA); sendCommand(0x12); // Pin Configuration
    sendCommand(0x81); sendCommand(0xCF); // Contrast Control
    sendCommand(0xD9); sendCommand(0xF1); // Pre-charge Period
    sendCommand(0xDB); sendCommand(0x40); // VCOMH Deselect Level
    sendCommand(0xA4);                     // Entire Display ON (Resume to RAM)
    sendCommand(0xA6);                     // Normal Display Mode
    sendCommand(0xAF);                     // Display ON
}

/**
 * Transferisce le 8 pagine (1024 byte) del frame buffer verso la RAM dello schermo
 */
void Ssd1306::updateDisplay() {
    for (uint8_t page = 0; page < 8; ++page) {
        sendCommand(0xB0 + page); // Set Page Start Address (0-7)
        sendCommand(0x00);        // Lower Column Start Address
        sendCommand(0x10);        // Higher Column Start Address

        char pageData[129];
        pageData[0] = 0x40; // Data Prefix (Co=0, D/C=1)
        memcpy(&pageData[1], _frameBuffer[page], 128);

        _i2c.write(SSD1306_I2C_ADDR, pageData, 129);
    }
}

// ============================================================================
// COMPOSIZIONE DELLE SCHERMATE GRAFICHE
// ============================================================================

/**
 * 1. Schermata Iniziale (STATE_IDLE)
 */
void Ssd1306::drawSplashScreen() {
    clearBuffer();

    // Titolo e Prompt centrati
    renderText(_frameBuffer, 31, 4, STRING_SPLASH_TITLE);
    renderBitmap(_frameBuffer, 58, 22, dinoBitmap, DINO_WIDTH, DINO_HEIGHT);
    renderText(_frameBuffer, 28, 52, STRING_SPLASH_PROMPT);

    updateDisplay();
}

/**
 * 2. Gameplay (STATE_PLAYING)
 */
void Ssd1306::drawGameScene(int16_t dinoY, int16_t obstacleX, uint32_t score, uint32_t highScore) {
    clearBuffer();

    // A. Status Bar in Alto (Score e High Score)
    char scoreBuffer[16];
    snprintf(scoreBuffer, sizeof(scoreBuffer), "HI:%04lu %04lu", highScore, score);
    renderText(_frameBuffer, 2, 2, scoreBuffer);

    // B. Terreno (Linea orizzontale a Y = GROUND_Y)
    for (int16_t x = 0; x < 128; ++x) {
        _frameBuffer[GROUND_Y / 8][x] |= (1 << (GROUND_Y % 8));
    }

    // C. Sprite Dino e Cactus
    renderBitmap(_frameBuffer, DINO_X_POS, dinoY, dinoBitmap, DINO_WIDTH, DINO_HEIGHT);
    if (obstacleX >= 0 && obstacleX < 128) {
        renderBitmap(_frameBuffer, obstacleX, OBSTACLE_GROUND_Y, cactusBitmap, CACTUS_WIDTH, CACTUS_HEIGHT);
    }

    updateDisplay();
}

/**
 * 3. Game Over (STATE_GAMEOVER)
 */
void Ssd1306::drawGameOverScreen(uint32_t finalScore) {
    clearBuffer();

    // A. Scritta "GAME OVER" in alto
    renderText(_frameBuffer, 37, 0, STRING_GAME_OVER);

    // B. Composizione dello Skull and Bones al centro
    renderSkullAndBonesAsset(_frameBuffer);

    // C. Punteggio Finale in basso
    char finalStr[20];
    snprintf(finalStr, sizeof(finalStr), "SCORE: %04lu", finalScore);
    renderText(_frameBuffer, 31, 56, finalStr);

    updateDisplay();
}