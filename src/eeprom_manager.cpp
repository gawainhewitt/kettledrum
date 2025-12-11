#include "eeprom_manager.h"
#include "config.h"
#include <EEPROM.h>

EEPROMManager::EEPROMManager() 
    : pendingWrite(false), writeScheduledTime(0) {
}

void EEPROMManager::begin() {
    // Nothing needed for Teensy EEPROM
}

bool EEPROMManager::validateNote(uint8_t note) {
    return note <= 127;
}

void EEPROMManager::initializeEEPROM(uint8_t drum1Note, uint8_t drum2Note) {
    EEPROM.write(EEPROM_ADDR_MAGIC, EEPROM_MAGIC_NUMBER);
    EEPROM.write(EEPROM_ADDR_DRUM1_NOTE, drum1Note);
    EEPROM.write(EEPROM_ADDR_DRUM2_NOTE, drum2Note);
}

bool EEPROMManager::loadNotes(uint8_t &drum1Note, uint8_t &drum2Note) {
    uint8_t magic = EEPROM.read(EEPROM_ADDR_MAGIC);
    
    if (magic == EEPROM_MAGIC_NUMBER) {
        // Valid EEPROM data exists
        drum1Note = EEPROM.read(EEPROM_ADDR_DRUM1_NOTE);
        drum2Note = EEPROM.read(EEPROM_ADDR_DRUM2_NOTE);
        
        // Validate the loaded values
        if (!validateNote(drum1Note)) {
            drum1Note = DEFAULT_DRUM1_NOTE;
        }
        if (!validateNote(drum2Note)) {
            drum2Note = DEFAULT_DRUM2_NOTE;
        }
        
        return true;
    } else {
        // First boot or corrupted EEPROM - use defaults
        drum1Note = DEFAULT_DRUM1_NOTE;
        drum2Note = DEFAULT_DRUM2_NOTE;
        
        // Initialize EEPROM with defaults
        initializeEEPROM(drum1Note, drum2Note);
        
        return false;
    }
}

void EEPROMManager::saveNote(int drumIndex, uint8_t note) {
    int address = (drumIndex == 0) ? EEPROM_ADDR_DRUM1_NOTE : EEPROM_ADDR_DRUM2_NOTE;
    
    // Read before write to minimize EEPROM wear
    uint8_t currentValue = EEPROM.read(address);
    if (currentValue != note) {
        EEPROM.write(address, note);
    }
}

void EEPROMManager::update(unsigned long currentTime, bool notesDirty, 
                          unsigned long lastNoteChange, uint8_t drum1Note, uint8_t drum2Note) {
    // Check if we need to schedule a write
    if (notesDirty && !pendingWrite) {
        pendingWrite = true;
        writeScheduledTime = lastNoteChange + EEPROM_WRITE_DELAY_MS;
    }
    
    // Execute pending write if time has elapsed
    if (pendingWrite && currentTime >= writeScheduledTime) {
        saveNote(0, drum1Note);
        saveNote(1, drum2Note);
        pendingWrite = false;
    }
}