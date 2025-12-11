#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include <Arduino.h>

class EEPROMManager {
public:
    EEPROMManager();
    void begin();
    
    // Load notes from EEPROM, returns true if valid data found
    bool loadNotes(uint8_t &drum1Note, uint8_t &drum2Note);
    
    // Save note to EEPROM (with read-before-write)
    void saveNote(int drumIndex, uint8_t note);
    
    // Check if write is needed and handle delayed write
    void update(unsigned long currentTime, bool notesDirty, 
                unsigned long lastNoteChange, uint8_t drum1Note, uint8_t drum2Note);

private:
    void initializeEEPROM(uint8_t drum1Note, uint8_t drum2Note);
    bool validateNote(uint8_t note);
    
    bool pendingWrite;
    unsigned long writeScheduledTime;
};

#endif // EEPROM_MANAGER_H