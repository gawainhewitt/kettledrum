# Teensy Drum Brain

A professional-quality electronic drum trigger system built on Teensy 4.0 with audio shield, featuring real-time wavetable synthesis and an intuitive OLED interface.

## Overview

This project implements a complete drum brain system that converts piezo sensor inputs from drum pads into realistic timpani sounds. The system uses embedded audio samples and hardware-accelerated wavetable synthesis to achieve low-latency, expressive drum triggering with independent pitch control per pad.

### OrchLab Project

This drum brain was developed as part of [OrchLab](https://orchlab.org/), a collaboration between the [London Philharmonic Orchestra](https://lpo.org.uk/) and [Drake Music](https://www.drakemusic.org/) to create accessible orchestral instruments. The goal is to provide a pair of accessible kettledrums (timpani) that can be played by disabled musicians while maintaining the expressive qualities and professional sound expected in an orchestral setting.

## Features

### Audio Processing
- **Wavetable synthesis** using Teensy Audio Library for realistic timpani sounds
- **Velocity-sensitive playback** responding to hit dynamics
- **Independent MIDI note assignment** per drum (pitch shifting from single sample set)
- **Master volume control** via potentiometer with real-time feedback
- **Dual-channel audio output** through Teensy Audio Shield (SGTL5000)

### Drum Triggering
- **Peak detection algorithm** for accurate hit capture
- **Configurable sensitivity** per drum (50-500 range)
- **Scan time and mask time** protection against double-triggering
- **Support for two independent drum inputs** (easily expandable)

### User Interface
- **128x64 OLED display** (I2C on bus 1) with multiple screens:
  - Idle screen with hit indicators
  - Menu system for MIDI note selection
  - Volume overlay
  - Sensitivity adjustment overlay
- **Five-button control** (directional cross + center button)
- **Three potentiometers**:
  - Pot 1: Drum 1 sensitivity
  - Pot 2: Drum 2 sensitivity
  - Pot 3: Master volume

### Data Persistence
- **EEPROM storage** for MIDI note assignments
- **Delayed write protection** (30-second delay after last change)
- **Configuration validation** with magic number check
- **Automatic fallback** to default values on corrupted data

## Hardware Requirements

### Core Components
- **Teensy 4.0** microcontroller
- **Teensy Audio Shield** (Rev D with SGTL5000)
- **128x64 OLED display** (SSD1306, I2C)
- **Two piezo sensor drum pads** (e.g., Alesis with optional mesh head upgrade)

### Control Interface
- **5x Tact switches** (directional cross layout)
- **3x Potentiometers** (10kΩ recommended)

### Piezo Protection Circuit (per drum)
Each piezo input requires proper signal conditioning to prevent damage and false triggers:

```
Piezo (+) ----[1MΩ resistor]---- Teensy Analog Pin (A0/A1)
                    |
                [1N4148 diode]
                    |
                   GND

Piezo (-) -------- GND
```

**Critical notes:**
- The 1MΩ series resistor provides current limiting and impedance matching
- The 1N4148 diode clamps negative voltage spikes to prevent ADC damage
- Proper impedance matching eliminates EMI-induced false triggering

### Pin Assignments

| Component | Pin | Notes |
|-----------|-----|-------|
| Drum 1 (Piezo) | A0 | With protection circuit |
| Drum 2 (Piezo) | A1 | With protection circuit |
| Pot 1 (Drum 1 Sens) | A11 | 0-3.3V range |
| Pot 2 (Drum 2 Sens) | A13 | 0-3.3V range |
| Pot 3 (Volume) | A12 | 0-3.3V range |
| Button UP | 9 | Active LOW |
| Button LEFT | 4 | Active LOW |
| Button CENTER | 2 | Active LOW |
| Button RIGHT | 5 | Active LOW |
| Button DOWN | 3 | Active LOW |
| OLED SDA | 18 (SDA1) | I2C Bus 1 |
| OLED SCL | 19 (SCL1) | I2C Bus 1 |

## Software Architecture

The project follows a modular, object-oriented design with clear separation of concerns:

### Core Classes

#### `DrumTrigger` (`drum_trigger.h/cpp`)
Handles piezo sensor input processing with peak detection algorithm:
- Threshold-based trigger detection
- Scan window for peak capture
- Mask time to prevent double-triggering
- Adjustable sensitivity per drum

#### `AudioManager` (`audio_manager.h/cpp`)
Manages audio synthesis and playback:
- Dual wavetable synthesizer instances
- Mixer for channel management
- MIDI note-based pitch shifting
- Volume control and velocity mapping
- Embedded timpani sample data

#### `DisplayManager` (`display_manager.h/cpp`)
Handles all OLED rendering using custom I2C bus 1 library:
- Splash screen
- Idle screen with hit indicators
- Menu system display
- Volume and sensitivity overlays
- Real-time hit dot animations

#### `InputControls` (`input_controls.h/cpp`)
Manages all user input devices:
- Button debouncing and press detection
- Potentiometer reading with change detection
- Noise filtering on analog inputs

#### `MenuSystem` (`menu_system.h/cpp`)
Implements the note selection interface:
- MIDI note range (C2-C4, notes 36-60)
- Per-drum note selection
- Auto-timeout after 15 seconds
- Dirty flag tracking for EEPROM writes

#### `EEPROMManager` (`eeprom_manager.h/cpp`)
Handles persistent configuration storage:
- Delayed write protection (30 seconds)
- Data validation with magic number
- Atomic read/write operations

### Audio Sample Format

The system uses the `simpletimp` instrument data, which is a soundfont-derived wavetable format compatible with the Teensy Audio Library. The samples are embedded in flash memory to avoid SPI bus interference from SD card operations.

**Why embedded samples?**
- SD card SPI activity creates electrical interference on analog inputs
- Flash storage eliminates continuous false triggering issues
- Instant access with no file system overhead

## Installation

### Prerequisites

- **PlatformIO** (VSCode extension or CLI)
- **Teensy 4.0 board definitions**
- **Custom U8g2 library fork** for I2C bus 1 support

### Build and Upload

1. Clone the repository:
```bash
git clone <repository-url>
cd teensy-drum-brain
```

2. Install dependencies:
```bash
pio lib install
```

3. Build and upload:
```bash
pio run --target upload
```

4. Monitor serial output (optional):
```bash
pio device monitor
```

### Library Dependencies

The project uses a custom fork of U8g2 that enables I2C bus 1 functionality:
```ini
lib_deps = 
    https://github.com/gawainhewitt/bus1_U8g2
```

This is necessary because the standard U8g2 library only supports I2C bus 0, which conflicts with the Teensy Audio Shield.

## Configuration

### Default Settings

Defined in `config.h`:

| Parameter | Default | Range | Description |
|-----------|---------|-------|-------------|
| `THRESHOLD` | 50 | 0-4095 | Minimum value to start trigger scan |
| `TRIGGER_VALUE` | 100 | 50-500 | Adjustable per-drum sensitivity |
| `SCAN_TIME` | 5 ms | - | Window to capture peak value |
| `MASK_TIME` | 50 ms | - | Dead time after trigger to prevent double-hits |
| `DEFAULT_DRUM1_NOTE` | 36 (C2) | 36-60 | Initial MIDI note for drum 1 |
| `DEFAULT_DRUM2_NOTE` | 43 (G2) | 36-60 | Initial MIDI note for drum 2 |

### Adjusting Sample Rate and Bit Depth

Audio parameters are configured in `audio_manager.cpp`:
- Sample rate: 44.1 kHz (standard)
- Bit depth: 16-bit
- Dual wavetable channels for polyphony

### MIDI Note Range

The menu system provides access to MIDI notes 36-60 (C2 through C4), which covers a two-octave range suitable for timpani voicing. Notes are displayed in standard musical notation (e.g., "C2", "F#3").

## Usage

### Initial Startup

1. Power on the system
2. Wait for splash screen (2 seconds)
3. System enters idle mode showing two drum indicators
4. Hit drums to trigger sounds with visual feedback

### Adjusting Volume

- Turn **Pot 3** (rightmost potentiometer)
- Volume overlay displays for 3 seconds
- Range: 0-100%
- Changes apply immediately

### Adjusting Drum Sensitivity

- Turn **Pot 1** for Drum 1 sensitivity
- Turn **Pot 2** for Drum 2 sensitivity
- Sensitivity overlay shows drum number and trigger threshold
- Range: 50-500 (lower = more sensitive)
- Overlay displays for 3 seconds

### Changing MIDI Notes

1. Press **CENTER** button to enter menu
2. Current drum indicator shows active selection
3. Use **LEFT/RIGHT** to switch between drums
4. Use **UP/DOWN** to adjust MIDI note
5. Hit drums to preview sound while in menu
6. Press **CENTER** again to exit, or wait 15 seconds for auto-timeout
7. Changes are saved to EEPROM after 30 seconds of inactivity

### Hit Indicators

- Solid dots appear on idle screen when drums are hit
- Dots remain visible for 250ms
- Provides visual confirmation of trigger detection
- Works in both idle and menu modes

## Technical Details

### Trigger Algorithm

The peak detection algorithm operates in two phases:

1. **Threshold Detection**: Continuously monitors analog input for values exceeding `THRESHOLD`
2. **Scan Window**: Once triggered, captures peak value over `SCAN_TIME` period
3. **Mask Period**: Ignores input for `MASK_TIME` after trigger to prevent retriggering

This approach ensures accurate capture of hit dynamics while rejecting false triggers from mechanical bounce and electrical noise.

### Audio Processing Pipeline

```
Piezo Input → ADC (12-bit) → Peak Detection → Velocity Scaling → 
Wavetable Synth → MIDI Pitch Shift → Mixer → I2S DAC → Audio Output
```

Each drum has its own wavetable instance, allowing simultaneous polyphonic playback with independent pitch control.

### EMI and Noise Mitigation

The system implements several strategies to prevent false triggering:

1. **High series resistance** (1MΩ) provides low-pass filtering of high-frequency EMI
2. **Clamping diodes** protect against voltage spikes
3. **Software filtering** with increased change thresholds on potentiometers (50 ADC counts)
4. **No SD card access** during normal operation eliminates SPI bus interference
5. **Initialization flags** prevent spurious overlay displays on startup

### Display Update Strategy

The display updates at approximately 20Hz (every 50ms) to balance responsiveness with processor load. Different display states are managed by a finite state machine:

- `STATE_IDLE`: Normal operation showing hit dots
- `STATE_VOLUME_OVERLAY`: Temporary volume display (3s timeout)
- `STATE_SENSITIVITY_OVERLAY`: Temporary sensitivity display (3s timeout)
- `STATE_MENU`: Note selection interface (15s timeout)

### EEPROM Write Protection

To extend EEPROM lifespan, writes are delayed by 30 seconds after the last configuration change. This prevents excessive wear from rapid adjustments while ensuring settings are preserved if the system is powered down.

## Serial Monitor Output

The system provides detailed logging via USB serial at 115200 baud:

```
Loaded notes from EEPROM
Drum 1: C2 (MIDI 36) | Drum 2: G2 (MIDI 43)
Drum trigger system ready!
Hit the drums or press CENTER to enter menu...
Volume: 0.75
Drum 1 Trigger: 150
```

## Troubleshooting

### No Sound Output
- Verify audio shield is properly seated
- Check SGTL5000 initialization in serial monitor
- Confirm headphone/line out connections
- Increase master volume (Pot 3)

### False Triggering / Self-Triggering
- Verify piezo protection circuit (1MΩ resistor + 1N4148 diode)
- Check for EMI sources near analog input wiring
- Ensure proper grounding of piezo shields
- Increase trigger threshold (Pot 1/2 to less sensitive)

### Pots Causing Unwanted Overlays
- System uses initialization flags to prevent startup noise
- Increase `POT_CHANGE_THRESHOLD` in config.h if needed (currently 50)

### Display Not Working
- Confirm OLED is on I2C bus 1 (pins 18/19, not 16/17)
- Check bus1_U8g2 library is properly installed
- Verify I2C address (typically 0x3C)

### Notes Not Saving
- Wait 30 seconds after last change before power-off
- Check serial output for EEPROM write confirmation
- Verify EEPROM is not write-protected

## Future Enhancements

Potential areas for expansion:

- [ ] Add more drum inputs (Teensy 4.0 has many analog pins available)
- [ ] Implement different instrument samples (kick, snare, cymbals)
- [ ] MIDI output capability for use with external sound modules
- [ ] Choke group support for hi-hats and cymbals
- [ ] Per-drum envelope shaping (attack, decay, release)
- [ ] Reverb and effects processing
- [ ] Metronome function with tempo control
- [ ] Practice patterns and play-along tracks
- [ ] Configuration presets (different kits)

## Credits

**Hardware**: Teensy 4.0, PJRC Audio Shield Rev D  
**Display Library**: Custom U8g2 fork for I2C bus 1 support  
**Audio Library**: Teensy Audio Library by Paul Stoffregen  
**Samples**: SimpleTimpani soundfont, derived from timpani sound by Shōtotsu ([CC0 License](https://creativecommons.org/publicdomain/zero/1.0/))  
**Original Sound**: https://freesound.org/people/Shōtotsu/sounds/688793/

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

Gawain Hewitt
