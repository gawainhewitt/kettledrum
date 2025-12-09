# Architecture Diagram

## Module Dependencies

```
                    main.cpp
                       |
        +--------------+--------------+
        |              |              |
        v              v              v
   config.h      DrumTrigger   InputControls
                      |              |
                      v              v
                 AudioManager   DisplayManager
                      |              |
                      v              v
              [Teensy Audio]    [OLED I2C]
```

## Data Flow

```
Piezo Sensors (A0, A1)
        |
        v
   DrumTrigger::update()
        |
        +---> Peak Detection
        |
        v
   main.cpp (triggered?)
        |
        +---> AudioManager::playDrum()
        |
        +---> DisplayManager::showDrumHit()


Potentiometers (A10-A12) & Buttons (2,3,4,5,9)
        |
        v
   InputControls::update()
        |
        +---> Change Detection
        |
        v
   main.cpp (button/pot changed?)
        |
        +---> DisplayManager::showButton()
        |
        +---> Serial Output
```

## Memory Layout

Each DrumTrigger instance maintains:
- Current state (scanning/idle)
- Peak value
- Timestamps
- Trigger flag

AudioManager maintains:
- Audio object instances (sine waves, mixer, I2S output)
- Audio connections (patch cords)

DisplayManager maintains:
- U8G2 display object
- I2C connection

InputControls maintains:
- Current pot values
- Previous pot values (for change detection)
- Button states (for edge detection)
