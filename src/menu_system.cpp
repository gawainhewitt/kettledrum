#include "menu_system.h"

MenuSystem::MenuSystem() 
    : state(MENU_IDLE), selectedDrum(0), 
      drum1Note(DEFAULT_DRUM1_NOTE), drum2Note(DEFAULT_DRUM2_NOTE),
      notesDirty(false), lastMenuActivity(0), lastNoteChange(0) {
}

void MenuSystem::begin(uint8_t initialDrum1Note, uint8_t initialDrum2Note) {
    drum1Note = initialDrum1Note;
    drum2Note = initialDrum2Note;
    notesDirty = false;
}

void MenuSystem::enterMenu() {
    state = MENU_ACTIVE;
    lastMenuActivity = millis();
}

void MenuSystem::exitMenu() {
    state = MENU_IDLE;
}

void MenuSystem::selectDrum(int drum) {
    if (drum >= 0 && drum <= 1) {
        selectedDrum = drum;
        lastMenuActivity = millis();
    }
}

void MenuSystem::adjustNote(int8_t delta) {
    uint8_t *note = (selectedDrum == 0) ? &drum1Note : &drum2Note;
    
    int newNote = *note + delta;
    
    // Clamp to valid MIDI range
    if (newNote < 0) newNote = 0;
    if (newNote > 127) newNote = 127;
    
    if (*note != newNote) {
        *note = newNote;
        notesDirty = true;
        lastNoteChange = millis();
        lastMenuActivity = millis();
    }
}

void MenuSystem::handleButtonPress(int buttonPin) {
    if (state == MENU_IDLE) {
        // Center button enters menu
        if (buttonPin == BTN_CENTER) {
            enterMenu();
        }
    } else if (state == MENU_ACTIVE) {
        lastMenuActivity = millis();
        
        if (buttonPin == BTN_CENTER) {
            exitMenu();
        } else if (buttonPin == BTN_UP) {
            selectDrum(0);
        } else if (buttonPin == BTN_DOWN) {
            selectDrum(1);
        } else if (buttonPin == BTN_LEFT) {
            adjustNote(-1);
        } else if (buttonPin == BTN_RIGHT) {
            adjustNote(1);
        }
    }
}

void MenuSystem::update(unsigned long currentTime) {
    // Check for menu timeout
    if (state == MENU_ACTIVE) {
        if (currentTime - lastMenuActivity > MENU_TIMEOUT_MS) {
            exitMenu();
        }
    }
}