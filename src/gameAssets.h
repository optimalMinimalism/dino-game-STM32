#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

#include "mbed.h"

// Dimensioni degli Sprite in Pixel
#define DINO_WIDTH       12
#define DINO_HEIGHT      12

#define CACTUS_WIDTH     8
#define CACTUS_HEIGHT    14

#define SKULL_WIDTH      16
#define SKULL_HEIGHT     16

// ============================================================================
// ASSETS ESPORTATI (DATI IN FLASH)
// ============================================================================
extern const uint16_t dinoBitmap[12];
extern const uint16_t cactusBitmap[14];
extern const uint16_t skullRows[16];

// Matrice delle tibie incrociate: {x0, y0, x1, y1}
extern const int16_t boneLines[2][4];

// Stringhe e Testi del Gioco
extern const char STRING_SPLASH_TITLE[];  // "DINO-RUNNER"
extern const char STRING_SPLASH_PROMPT[]; // "PRESS BUTTON"
extern const char STRING_GAME_OVER[];    // "GAME OVER"
extern const char STRING_SCORE_PREFIX[];  // "SCORE: "

// ============================================================================
// METODI DI RENDERING TESTO & ASSETS SUL FRAMEBUFFER
// ============================================================================

/**
 * Renderizza una stringa di testo sul frame buffer (128x64) a partire da (x, y)
 */
void renderText(uint8_t frameBuffer[8][128], int16_t x, int16_t y, const char *str);

/**
 * Renderizza uno sprite bitmap standard nel buffer
 */
void renderBitmap(uint8_t frameBuffer[8][128], int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h);

/**
 * Renderizza la composizione completa dello Skull and Bones
 */
void renderSkullAndBonesAsset(uint8_t frameBuffer[8][128]);

#endif // GAME_ASSETS_H