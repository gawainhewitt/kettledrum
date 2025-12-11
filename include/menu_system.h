#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <Arduino.h>
#include "config.h"

enum MenuState {
    MENU_IDLE,
    MENU_ACTIVE
};

class MenuSystem {
public:
    MenuSystem();
    
    void begin(uint8_t initialDrum1Note, uint8_t initialDrum2Note);
    void update(unsigned long currentTime);
    
    // Button handling
    void handleButtonPress(int buttonPin);
    
    // State queries
    bool isMenuActive() const { return state == MENU_ACTIVE; }
    int getSelectedDrum() const { return selectedDrum; }
    uint8_t getDrum1Note() const { return drum1Note; }
    uint8_t getDrum2Note() const { return drum2Note; }
    bool areNotesDirty() const { return notesDirty; }
    unsigned long getLastNoteChange() const { return lastNoteChange; }
    
    // State setters
    void setDrum1Note(uint8_t note) { drum1Note = note; }
    void setDrum2Note(uint8_t note) { drum2Note = note; }
    void clearDirtyFlag() { notesDirty = false; }
    
private:
    MenuState state;
    int selectedDrum;  // 0 or 1
    uint8_t drum1Note;
    uint8_t drum2Note;
    bool notesDirty;
    unsigned long lastMenuActivity;
    unsigned long lastNoteChange;
    
    void enterMenu();
    void exitMenu();
    void selectDrum(int drum);
    void adjustNote(int8_t delta);
};

#endif // MENU_SYSTEM_H