#include "mbed.h"
#include "USBSerial.h"
#include "gameAssets.h"
#include "gameEngine.h"
#include "ssd1306.h"

// ============================================================================
// ASSEGNAZIONE PIN SCHEDA ELIOT (Schematic Rev 1.1)
// ============================================================================
#define ELIOT_PIN_USER_KEY   PC_13   // J21: Pulsante Utente SW2 (Active LOW)
#define ELIOT_PIN_STATUS_LED PA_1    // D3: +3V3 -> LED -> R39 -> LED_OUT (active LOW)
#define ELIOT_PIN_I2C3_SDA   PC_9    // J7 Pin 1: Display I2C3 SDA
#define ELIOT_PIN_I2C3_SCL   PA_8    // J7 Pin 2: Display I2C3 SCL

// Thread Flag per risvegliare il task di gestione dell'input
#define FLAG_BTN_PRESSED     (1 << 0)

// ============================================================================
// RISORSE HARDWARE & RTOS PERIPHERALS
// ============================================================================
// The Nucleo pin map omits PA8 as I2C3 SCL because it is used for MCO there.
// Eliot wires PA8 to the OLED, so describe that hardware connection explicitly.
static const i2c_pinmap_t eliotI2c3Pinmap = {
    I2C_3,
    ELIOT_PIN_I2C3_SDA, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3),
    ELIOT_PIN_I2C3_SCL, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)
};

static mbed::InterruptIn userButton(ELIOT_PIN_USER_KEY);
static mbed::DigitalOut  statusLed(ELIOT_PIN_STATUS_LED, 0); // HIGH = LED off
static mbed::I2C         i2c3Bus(eliotI2c3Pinmap);

// Driver Display e Game Engine
static Ssd1306    display(i2c3Bus);
static GameEngine engine;

// Thread RTOS e Mutex per protezione concorrenza sul Game Engine
static rtos::Thread inputThread(osPriorityHigh, 1024, nullptr, "InputTask");
static rtos::Thread heartbeatThread(osPriorityLow, 512, nullptr, "HeartbeatTask");
static rtos::Mutex  engineMutex;

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

    // Probe both common SSD1306 addresses before initializing the display.
    // A NACK at both addresses points to wiring, power, pull-ups, or pin routing.
    const char displayOff[] = {0x00, static_cast<char>(0xAE)};
    const bool oledAt3C = i2c3Bus.write(0x3C << 1, displayOff, sizeof(displayOff)) == 0;
    const bool oledAt3D = i2c3Bus.write(0x3D << 1, displayOff, sizeof(displayOff)) == 0;
    printf("OLED 0x3C: %s\n", oledAt3C ? "ACK" : "no ACK");
    printf("OLED 0x3D: %s\n", oledAt3D ? "ACK" : "no ACK");
    fflush(stdout);

    // J2 is native USB, so it needs a USB CDC device. Do not wait for a PC.
    // static USBSerial usbSerial(false);
    // usbSerial.connect();

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
    uint32_t usbLogFrame = 0;
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

        // Repeat diagnostics so the USB monitor can be opened after boot.
        if (++usbLogFrame >= 20) {
            usbLogFrame = 0;
            // Stampa inviata direttamente sul canale ITM (Pin SWO / J1 Pin 6)
            printf("eLioT alive; OLED 0x3C: %s, 0x3D: %s\r\n",
                oledAt3C ? "ACK" : "no ACK",
                oledAt3D ? "ACK" : "no ACK");
        }

        // Frame rate target: 20 FPS (50ms per frame)
        ThisThread::sleep_for(50ms);
    }
}