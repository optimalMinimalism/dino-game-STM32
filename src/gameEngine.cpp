#include "gameEngine.h"

// ============================================================================
// INIZIALIZZAZIONE ENGINE E RESET PARTITA
// ============================================================================
void initGameEngine(GameEngine &engine) {
    engine.state = STATE_IDLE;
    engine.score = 0;
    engine.highScore = 0;

    // Reset posizione Dino
    engine.dino.y = DINO_GROUND_Y;
    engine.dino.vy = 0;
    engine.dino.isGrounded = true;

    // Reset posizione Ostacolo
    engine.obstacle.x = OBSTACLE_SPAWN_X;
    engine.obstacle.y = OBSTACLE_GROUND_Y;
    engine.obstacle.active = false;
}

// ============================================================================
// GESTIONE INPUT (USER_KEY PC_13)
// ============================================================================
void handleButtonPress(GameEngine &engine) {
    switch (engine.state) {
        case STATE_IDLE:
            // Da IDLE a PLAYING: Avvia la partita
            engine.score = 0;
            engine.dino.y = DINO_GROUND_Y;
            engine.dino.vy = 0;
            engine.dino.isGrounded = true;
            
            engine.obstacle.x = OBSTACLE_SPAWN_X;
            engine.obstacle.active = true;
            
            engine.state = STATE_PLAYING;
            break;

        case STATE_PLAYING:
            // Salto: applica l'impulso verticale solo se il Dino e' a terra
            if (engine.dino.isGrounded) {
                engine.dino.vy = JUMP_IMPULSE;
                engine.dino.isGrounded = false;
            }
            break;

        case STATE_GAMEOVER:
            // Da GAMEOVER a IDLE: Ripristina e torna alla schermata iniziale
            engine.dino.y = DINO_GROUND_Y;
            engine.dino.vy = 0;
            engine.dino.isGrounded = true;
            
            engine.obstacle.x = OBSTACLE_SPAWN_X;
            engine.obstacle.active = false;
            
            engine.state = STATE_IDLE;
            break;
    }
}

// ============================================================================
// BOUNDING BOX COLLISION DETECTION
// ============================================================================
bool checkCollision(const Dino &dino, const Obstacle &obstacle) {
    if (!obstacle.active) {
        return false;
    }

    // Coordinate Bounding Box Dino
    int16_t dinoLeft   = DINO_X_POS;
    int16_t dinoRight  = DINO_X_POS + DINO_WIDTH;
    int16_t dinoTop    = dino.y;
    int16_t dinoBottom = dino.y + DINO_HEIGHT;

    // Coordinate Bounding Box Ostacolo (Cactus)
    int16_t obsLeft   = obstacle.x;
    int16_t obsRight  = obstacle.x + CACTUS_WIDTH;
    int16_t obsTop    = obstacle.y;
    int16_t obsBottom = obstacle.y + CACTUS_HEIGHT;

    // Sovrapposizione lungo gli assi X e Y
    bool overlapX = (dinoRight > obsLeft) && (dinoLeft < obsRight);
    bool overlapY = (dinoBottom > obsTop) && (dinoTop < obsBottom);

    return (overlapX && overlapY);
}

// ============================================================================
// AGGIORNAMENTO LOGICA E FISICA (CHIAMATO FRAME PER FRAME)
// ============================================================================
void updateGameLogic(GameEngine &engine) {
    if (engine.state != STATE_PLAYING) {
        return;
    }

    // 1. Fisica del Dinosauro (Salto e Gravita')
    engine.dino.y += engine.dino.vy;
    engine.dino.vy += GRAVITY;

    // Controllo atterraggio sul terreno
    if (engine.dino.y >= DINO_GROUND_Y) {
        engine.dino.y = DINO_GROUND_Y;
        engine.dino.vy = 0;
        engine.dino.isGrounded = true;
    }

    // 2. Movimento dell'Ostacolo
    if (engine.obstacle.active) {
        engine.obstacle.x -= OBSTACLE_SPEED;

        // Se l'ostacolo esce dallo schermo a sinistra, fa il respawn
        if (engine.obstacle.x + CACTUS_WIDTH < 0) {
            engine.obstacle.x = OBSTACLE_SPAWN_X;
        }
    }

    // 3. Incremento Punteggio e High Score
    engine.score++;
    if (engine.score > engine.highScore) {
        engine.highScore = engine.score;
    }

    // 4. Verifica Collisione
    if (checkCollision(engine.dino, engine.obstacle)) {
        engine.state = STATE_GAMEOVER;
    }
}