# Drum Brain Project - Modular Structure

## Overview
This is a reorganized version of the kettle drum brain code, split into modular components for better organization and maintainability.

## Project Structure

```
drum_brain/
├── include/              # Header files (.h)
│   ├── config.h          # Hardware pin definitions and constants
│   ├── drum_trigger.h    # Drum trigger detection class
│   ├── audio_manager.h   # Audio synthesis and playback
│   ├── display_manager.h # OLED display control
│   └── input_controls.h  # Button and potentiometer handling
├── src/                  # Implementation files (.cpp)
│   ├── main.cpp          # Main application
│   ├── drum_trigger.cpp
│   ├── audio_manager.cpp
│   ├── display_manager.cpp
│   └── input_controls.cpp
└── platformio.ini        # PlatformIO configuration
```

## Module Descriptions

### config.h
Contains all hardware pin assignments and configuration constants. Change pin numbers and timing parameters here.

### DrumTrigger
Handles piezo sensor reading and peak detection for a single drum. Each drum gets its own instance with independent state tracking.

### AudioManager
Manages the Teensy Audio Library setup, audio routing through the mixer, and sound playback for both drums.

### DisplayManager
Controls the OLED display using I2C bus 1. Provides methods for showing splash screen, drum hits, and button presses.

### InputControls
Manages all user input: 3 potentiometers and 5 tact switches. Handles debouncing and change detection.

## Benefits of This Structure

1. **Separation of Concerns**: Each module handles one specific aspect of functionality
2. **Reusability**: Classes can be easily instantiated multiple times (e.g., adding more drums)
3. **Maintainability**: Changes to one subsystem don't affect others
4. **Testability**: Individual modules can be tested independently
5. **Readability**: main.cpp is now clean and shows the high-level program flow

## Usage

The main.cpp file creates instances of each module and coordinates them in the loop. To add features:

- **Add more drums**: Create additional DrumTrigger instances
- **Change audio**: Modify AudioManager class
- **Add UI features**: Extend DisplayManager
- **Add controls**: Extend InputControls class
