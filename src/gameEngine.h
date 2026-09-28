#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "mbed.h"
#include "gameAssets.h"

// ============================================================================
// COSTANTI FISICHE E DI RENDERING
// ============================================================================
#define GROUND_Y             54    // Coordinate Y della linea del terreno
#define DINO_X_POS           12    // Posizione X fissa del Dino
#define DINO_GROUND_Y        (GROUND_Y - DINO_HEIGHT) // Y del Dino quando è a terra (42)

#define JUMP_IMPULSE         -6    // Velocità iniziale del salto (verso l'alto)
#define GRAVITY               1    // Accelerazione di gravità applicata frame per frame

#define OBSTACLE_SPAWN_X    120    // Coordinata X di partenza degli ostacoli
#define OBSTACLE_GROUND_Y   (GROUND_Y - CACTUS_HEIGHT) // Y dell'ostacolo fissa a terra (40)
#define OBSTACLE_SPEED        3    // Pixel di avanzamento verso sinistra per frame

// ============================================================================
// ENUMERAZIONI (FSM GAME STATES)
// ============================================================================
// logica delle transizioni STATE_IDLE -> STATE_PLAYING -> STATE_GAMEOVER -> STATE_IDLE
enum GameState {
    STATE_IDLE = 0,   // Schermata iniziale "DINO-RUNNER" / "PRESS BUTTON"
    STATE_PLAYING,      // Gameplay attivo (Fisica Dino, Ostacoli, Punteggio)
    STATE_GAMEOVER      // Morte (Skull and Bones + Final Score + Restart)
};

// ============================================================================
// STRUTTURE DATI ENTITÀ DI GIOCO
// ============================================================================

/**
 * Entità Dinosauro (Giocatore)
 */
struct Dino {
    int16_t y;          // Coordinata Y corrente
    int16_t vy;         // Velocità verticale corrente
    bool isGrounded;    // true se si trova a terra (può saltare)
};

/**
 * Entità Ostacolo (Cactus)
 */
struct Obstacle {
    int16_t x;          // Coordinata X corrente
    int16_t y;          // Coordinata Y corrente (fissa a terra)
    bool active;        // Stato di presenza dello sprite a schermo
};

/**
 * Struttura di Stato Principale dell'Engine di Gioco
 */
struct GameEngine {
    GameState state;      // Stato corrente della FSM
    Dino dino;            // Oggetto Giocatore
    Obstacle obstacle;    // Oggetto Ostacolo
    uint32_t score;       // Punteggio della partita corrente
    uint32_t highScore;   // Punteggio massimo ottenuto nella sessione
};

// ============================================================================
// PROTOTIPI DELLE FUNZIONI DELL'ENGINE
// ============================================================================

/**
 * Inizializza o ripristina la struttura dell'Engine di Gioco
 */
void initGameEngine(GameEngine &engine);

/**
 * Gestisce l'evento di pressione del pulsante USER_KEY (PC_13)
 */
void handleButtonPress(GameEngine &engine);

/**
 * Esegue un ciclo di calcolo della fisica e aggiornamento di stato (chiamata periodicamente)
 */
void updateGameLogic(GameEngine &engine);

/**
 * Verifica se i Bounding Box del Dino e dell'Ostacolo si sovrappongono
 */
bool checkCollision(const Dino &dino, const Obstacle &obstacle);

#endif // GAME_ENGINE_H