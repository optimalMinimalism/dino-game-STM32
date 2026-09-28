#include "mbed.h"
#include "gameAssets.h"
#include "gameEngine.h"
#include "ssd1306.h"

// ============================================================================
// ASSEGNAZIONE PIN SCHEDA ELIOT (Schematic Rev 1.1)
// ============================================================================
#define ELIOT_PIN_USER_KEY   PC_13   // J21: Pulsante Utente SW2 (Active LOW)
#define ELIOT_PIN_STATUS_LED PC_10   // D3:  Status LED (Net RS_DIR)
#define ELIOT_PIN_I2C3_SDA   PC_9    // J7 Pin 1: Display I2C3 SDA
#define ELIOT_PIN_I2C3_SCL   PA_8    // J7 Pin 2: Display I2C3 SCL

// Thread Flag per risvegliare il task di gestione dell'input
#define FLAG_BTN_PRESSED     (1 << 0)

// ============================================================================
// RISORSE HARDWARE & RTOS PERIPHERALS
// ============================================================================
static mbed::InterruptIn userButton(ELIOT_PIN_USER_KEY);
static mbed::DigitalOut  statusLed(ELIOT_PIN_STATUS_LED, 0);
static mbed::I2C         i2c3Bus(ELIOT_PIN_I2C3_SDA, ELIOT_PIN_I2C3_SCL);

// Driver Display e Game Engine
static Ssd1306    display(i2c3Bus);
static GameEngine engine;

// Thread RTOS e Mutex per protezione concorrenza sul Game Engine
static mbed::Thread inputThread(osPriorityHigh, 1024, nullptr, "InputTask");
static mbed::Thread heartbeatThread(osPriorityLow, 512, nullptr, "HeartbeatTask");
static mbed::Mutex  engineMutex;

// ============================================================================
// INTERRUPT SERVICE ROUTINE (ISR)
// ============================================================================
/**
 * ISR minima: segnale rapido verso l'Input Thread via RTOS Flag
 */
void onButtonFallIsr() {
    inputThread.flags_set(FLAG_BTN_PRESSED);
}

// ============================================================================
// TASK 1: INPUT CONTROLLER THREAD
// ============================================================================
void inputTask() {
    while (true) {
        // Il thread dorme in low-power finché l'ISR non imposta la flag
        ThisThread::flags_wait_all(FLAG_BTN_PRESSED);

        // Debounce hardware / software
        ThisThread::sleep_for(40ms);

        // Verifica stato effettivo (attivo Basso)
        if (userButton.read() == 0) {
            engineMutex.lock();
            handleButtonPress(engine);
            engineMutex.unlock();
        }
    }
}

// ============================================================================
// TASK 2: SYSTEM HEARTBEAT THREAD (LED D3)
// ============================================================================
void heartbeatTask() {
    while (true) {
        // In gioco il LED raddoppia la frequenza
        uint32_t delayMs = (engine.state == STATE_PLAYING) ? 250 : 500;
        statusLed = !statusLed;
        ThisThread::sleep_for(Kernel::Clock::duration_u32(delayMs));
    }
}

// ============================================================================
// MAIN THREAD: GAME ENGINE LOOP & DISPLAY RENDER (~20 FPS)
// ============================================================================
int main() {
    printf("\n===========================================\n");
    printf("  ELIOT Board - Mbed OS Dino Runner Game   \n");
    printf("===========================================\n");

    // 1. Inizializzazione Display OLED J7
    display.init();

    // 2. Inizializzazione FSM Game Engine
    initGameEngine(engine);

    // 3. Configurazione Interrupt Pulsante J21 (Active LOW -> Fall)
    userButton.fall(&onButtonFallIsr);

    // 4. Avvio dei Thread secondari RTOS
    inputThread.start(inputTask);
    heartbeatThread.start(heartbeatTask);

    // 5. Loop Principale di Rendering
    while (true) {
        engineMutex.lock();

        // A. Calcolo logica di gioco e fisica se in fase di PLAYING
        updateGameLogic(engine);

        // B. Rendering sul Frame Buffer in base allo stato FSM
        switch (engine.state) {
            case STATE_IDLE:
                display.drawSplashScreen();
                break;

            case STATE_PLAYING:
                display.drawGameScene(
                    engine.dino.y, 
                    engine.obstacle.x, 
                    engine.score, 
                    engine.highScore
                );
                break;

            case STATE_GAMEOVER:
                display.drawGameOverScreen(engine.score);
                break;
        }

        engineMutex.unlock();

        // Frame rate target: 20 FPS (50ms per frame)
        ThisThread::sleep_for(50ms);
    }
}