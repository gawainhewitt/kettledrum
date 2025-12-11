#ifndef CONFIG_H
#define CONFIG_H

// Drum trigger pins
const int DRUM_PIN_1 = A0;
const int DRUM_PIN_2 = A1;

// Trigger parameters
const int THRESHOLD = 50;
const int TRIGGER_VALUE = 100;
const int SCAN_TIME = 5;
const int MASK_TIME = 50;

// Potentiometer pins
const int POT_PIN_1 = A11;
const int POT_PIN_2 = A13;
const int POT_PIN_3 = A12;
const int POT_READ_INTERVAL = 100;
const int POT_CHANGE_THRESHOLD = 15;

// Tact switch pins
const int BUTTON_PINS[] = {2, 3, 4, 5, 9};
const int NUM_BUTTONS = 5;

// Button position mappings (cross layout)
#define BTN_UP 9
#define BTN_LEFT 4
#define BTN_CENTER 2
#define BTN_RIGHT 5
#define BTN_DOWN 3

// EEPROM Configuration
#define EEPROM_MAGIC_NUMBER 0x42
#define EEPROM_ADDR_MAGIC 0
#define EEPROM_ADDR_DRUM1_NOTE 1
#define EEPROM_ADDR_DRUM2_NOTE 2

// Default MIDI Notes (Perfect Fifth: C2 and G2)
#define DEFAULT_DRUM1_NOTE 36  // C2
#define DEFAULT_DRUM2_NOTE 43  // G2

// Menu Timeouts (milliseconds)
#define MENU_TIMEOUT_MS 15000
#define OVERLAY_TIMEOUT_MS 3000
#define EEPROM_WRITE_DELAY_MS 30000

// Hit Dot Display Duration
#define HIT_DOT_DURATION_MS 250

// Convert MIDI note number to note name string (e.g., 60 -> "C3")
inline String midiToNoteName(uint8_t midiNote) {
    const char* noteNames[] = {"C", "C#", "D", "D#", "E", "F", 
                                "F#", "G", "G#", "A", "A#", "B"};
    int octave = (midiNote / 12) - 2;  // MIDI 60 = C3
    int noteIndex = midiNote % 12;
    return String(noteNames[noteIndex]) + String(octave);
}

#endif // CONFIG_H
