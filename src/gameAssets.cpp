#include "gameAssets.h"
#include <cstring>

// ----------------------------------------------------------------------------
// 1. STRINGHE DEL GIOCO
// ----------------------------------------------------------------------------
const char STRING_SPLASH_TITLE[]  = "DINO-RUNNER";
const char STRING_SPLASH_PROMPT[] = "PRESS BUTTON";
const char STRING_GAME_OVER[]     = "GAME OVER";
const char STRING_SCORE_PREFIX[]  = "SCORE:";

// ----------------------------------------------------------------------------
// 2. SPRITE & BITMAPS (Dino, Cactus, Teschio, Tibie)
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// SPRITE DINOSAURO (12x12 Pixel)
// ----------------------------------------------------------------------------
const uint16_t dinoBitmap[12] = {
    0x03E0, // ...#####.....
    0x02F0, // ...#.#####...
    0x03F0, // ...######....
    0x03C0, // ...####......
    0x07E0, // ..#######....
    0x0FE0, // .########....
    0x0FE0, // .########....
    0x07C0, // ..#####......
    0x0240, // ...#..#......
    0x0240, // ...#..#......
    0x0360, // ...##.##.....
    0x0000  // .............
};

// ----------------------------------------------------------------------------
//SPRITE CACTUS / OSTACOLO (8x14 Pixel)
// ----------------------------------------------------------------------------
const uint16_t cactusBitmap[14] = {
    0x1800, // ...##...
    0x5A00, // .#.#..#.
    0x5A00, // .#.#..#.
    0x7E00, // .######.
    0x3C00, // ..####..
    0x1800, // ...##...
    0x1800, // ...##...
    0x1800, // ...##...
    0x1800, // ...##...
    0x1800, // ...##...
    0x1800, // ...##...
    0x1800, // ...##...
    0x1800, // ...##...
    0x0000  // ........
};

const uint16_t skullRows[16] = {
    0x07E0, 0x1FF8, 0x3FFC, 0x7FFE,
    0x7FFE, 0x70C2, 0x70C2, 0x3FFC,
    0x1FF8, 0x0CC0, 0x0660, 0x0FF0,
    0x0A50, 0x0A50, 0x0FF0, 0x0000
};

const int16_t boneLines[2][4] = {
    {38, 52, 90, 26},
    {38, 26, 90, 52}
};

// ----------------------------------------------------------------------------
// 3. FONT 5x7 COMPATTA (Lettere maiuscole A-Z, Numeri 0-9 e Simboli : - Spazio)
// ----------------------------------------------------------------------------
static const uint8_t font5x7[][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // '0'
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // '1'
    {0x42, 0x61, 0x51, 0x49, 0x46}, // '2'
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // '3'
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // '4'
    {0x27, 0x45, 0x45, 0x45, 0x39}, // '5'
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // '6'
    {0x01, 0x71, 0x09, 0x05, 0x03}, // '7'
    {0x36, 0x49, 0x49, 0x49, 0x36}, // '8'
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // '9'
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, // 'A'
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 'B'
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 'C'
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 'D'
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 'E'
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 'F'
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 'G'
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 'H'
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 'I'
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 'J'
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 'K'
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 'L'
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 'M'
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 'N'
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 'O'
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 'P'
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 'Q'
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 'R'
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 'S'
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 'T'
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 'U'
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 'V'
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 'W'
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 'X'
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 'Y'
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 'Z'
    {0x00, 0x36, 0x36, 0x00, 0x00}, // ':'
    {0x08, 0x08, 0x08, 0x08, 0x08}, // '-'
    {0x00, 0x00, 0x00, 0x00, 0x00}  // ' ' (Spazio)
};

static int getCharIndex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    if (c == ':') return 36;
    if (c == '-') return 37;
    return 38; // Spazio
}

// Helper per impostare pixel sul framebuffer
static void setPixelBuffer(uint8_t fb[8][128], int16_t x, int16_t y) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;
    fb[y / 8][x] |= (1 << (y % 8));
}

// ----------------------------------------------------------------------------
// 4. LOGICA DI RENDERING TESTO E ASSETS
// ----------------------------------------------------------------------------
void renderText(uint8_t frameBuffer[8][128], int16_t x, int16_t y, const char *str) {
    int16_t cursorX = x;
    while (*str) {
        int idx = getCharIndex(*str);
        for (uint8_t col = 0; col < 5; ++col) {
            uint8_t line = font5x7[idx][col];
            for (uint8_t row = 0; row < 7; ++row) {
                if (line & (1 << row)) {
                    setPixelBuffer(frameBuffer, cursorX + col, y + row);
                }
            }
        }
        cursorX += 6; // 5 pixel larghezza + 1 pixel spaziatura
        str++;
    }
}

void renderBitmap(uint8_t frameBuffer[8][128], int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h) {
    for (uint8_t row = 0; row < h; ++row) {
        uint16_t line = bitmap[row];
        for (uint8_t col = 0; col < w; ++col) {
            if (line & (1 << (15 - col))) {
                setPixelBuffer(frameBuffer, x + col, y + row);
            }
        }
    }
}

void renderSkullAndBonesAsset(uint8_t frameBuffer[8][128]) {
    // Aste e pomelli tibie
    for (uint8_t b = 0; b < 2; ++b) {
        int16_t x0 = boneLines[b][0], y0 = boneLines[b][1];
        int16_t x1 = boneLines[b][2], y1 = boneLines[b][3];

        for (int16_t step = 0; step <= 52; ++step) {
            int16_t x = x0 + (x1 - x0) * step / 52;
            int16_t y = y0 + (y1 - y0) * step / 52;
            for (int16_t offset = -1; offset <= 1; ++offset) {
                setPixelBuffer(frameBuffer, x + offset, y);
                setPixelBuffer(frameBuffer, x, y + offset);
            }
        }

        const int16_t ends[2][2] = {{x0, y0}, {x1, y1}};
        for (uint8_t end = 0; end < 2; ++end) {
            for (int16_t dy = -3; dy <= 3; ++dy) {
                for (int16_t dx = -3; dx <= 3; ++dx) {
                    if (dx * dx + dy * dy <= 9) {
                        setPixelBuffer(frameBuffer, ends[end][0] + dx, ends[end][1] + dy);
                    }
                }
            }
        }
    }

    // Teschio 16x16 scalato 2x centrato
    for (uint8_t sourceY = 0; sourceY < 16; ++sourceY) {
        for (uint8_t sourceX = 0; sourceX < 16; ++sourceX) {
            if ((skullRows[sourceY] & (1U << (15U - sourceX))) == 0U) continue;

            for (uint8_t dy = 0; dy < 2; ++dy) {
                for (uint8_t dx = 0; dx < 2; ++dx) {
                    setPixelBuffer(frameBuffer, 48 + sourceX * 2 + dx, 8 + sourceY * 2 + dy);
                }
            }
        }
    }
}