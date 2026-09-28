# dino-game-STM32

## File Tree
├── arc.txt                         <-- Mbed OS thread diagram
├── platformio.ini                  <-- PlatformIO configuration
├── mbedApp.json                    <-- Mbed OS target overrides & settings
├── scripts/
│   └── mbedPythonCompat.py         <-- Python 3.10+ compatibility script
└── src/
    ├── main.cpp                    <-- Main entry point & thread initializations
    ├── ssd1306.h                   <-- SSD1306 display driver header
    ├── ssd1306.cpp                 <-- SSD1306 display driver & Skull and Bones 
    ├── gameEngine.h                <-- Dino game logic & state header
    └── gameEngine.cpp              <-- Physics, collision detection & frame rendering