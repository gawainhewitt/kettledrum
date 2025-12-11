#include <Arduino.h>
#include "config.h"
#include "drum_trigger.h"
#include "audio_manager.h"
#include "display_manager.h"
#include "input_controls.h"
#include "menu_system.h"
#include "eeprom_manager.h"

// Create instances
DrumTrigger drum1(DRUM_PIN_1, 1);
DrumTrigger drum2(DRUM_PIN_2, 2);
AudioManager audio;
DisplayManager display;
InputControls inputs;
MenuSystem menu;
EEPROMManager eepromManager;

// Pot state tracking with initialization flags
int lastPot3ForVolume = -1;
int lastPot1ForSensitivity = -1;
int lastPot2ForSensitivity = -1;
bool pot3Initialized = false;  // NEW
bool pot1Initialized = false;  // NEW
bool pot2Initialized = false;  // NEW

// Hit timing for visual feedback
unsigned long drum1HitTime = 0;
unsigned long drum2HitTime = 0;
unsigned long lastDisplayUpdate = 0;

// Display state
enum DisplayState {
  STATE_IDLE,
  STATE_VOLUME_OVERLAY,
  STATE_SENSITIVITY_OVERLAY,
  STATE_MENU
};

DisplayState displayState = STATE_IDLE;
unsigned long overlayStartTime = 0;
int overlayDrumIndex = 0;

void updateDisplay() {
  unsigned long currentTime = millis();
  
  // Check for overlay timeout
  if ((displayState == STATE_VOLUME_OVERLAY || displayState == STATE_SENSITIVITY_OVERLAY) &&
      (currentTime - overlayStartTime > OVERLAY_TIMEOUT_MS)) {
    displayState = STATE_IDLE;
  }
  
  // Calculate hit states
  bool drum1Active = (currentTime - drum1HitTime < HIT_DOT_DURATION_MS);
  bool drum2Active = (currentTime - drum2HitTime < HIT_DOT_DURATION_MS);
  
  // Render based on current state
  switch (displayState) {
    case STATE_IDLE:
      display.showIdleScreen(drum1Active, drum2Active);
      break;
      
    case STATE_VOLUME_OVERLAY:
      // Volume overlay doesn't show dots currently - could be enhanced
      break;
      
    case STATE_SENSITIVITY_OVERLAY:
      // Sensitivity overlay doesn't show dots currently - could be enhanced
      break;
      
    case STATE_MENU:
      display.showMenu(menu.getSelectedDrum(), 
                      menu.getDrum1Note(), 
                      menu.getDrum2Note(),
                      drum1Active,
                      drum2Active);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  
  // Initialize all subsystems
  drum1.begin();
  drum2.begin();
  audio.begin();
  display.begin();
  inputs.begin();
  eepromManager.begin();
  
  // Load notes from EEPROM
  uint8_t drum1Note, drum2Note;
  bool validData = eepromManager.loadNotes(drum1Note, drum2Note);
  
  if (validData) {
    Serial.println("Loaded notes from EEPROM");
  } else {
    Serial.println("Using default notes");
  }
  
  // Initialize menu system with loaded notes
  menu.begin(drum1Note, drum2Note);
  
  // Set audio manager notes
  audio.setDrum1Note(drum1Note);
  audio.setDrum2Note(drum2Note);
  
  Serial.print("Drum 1: ");
  Serial.print(midiToNoteName(drum1Note));
  Serial.print(" (MIDI ");
  Serial.print(drum1Note);
  Serial.print(") | Drum 2: ");
  Serial.print(midiToNoteName(drum2Note));
  Serial.print(" (MIDI ");
  Serial.print(drum2Note);
  Serial.println(")");
  
  // Show splash screen
  display.showSplash();
  Serial.println("Drum trigger system ready!");
  Serial.println("Hit the drums or press CENTER to enter menu...");
  
  delay(2000);
  
  // Initialize pot tracking values
  lastPot3ForVolume = inputs.getPot3Value();
  lastPot1ForSensitivity = inputs.getPot1Value();
  lastPot2ForSensitivity = inputs.getPot2Value();
  
  // Switch to idle screen after splash
  displayState = STATE_IDLE;
  display.showIdleScreen(false, false);
}

void loop() {
  unsigned long currentTime = millis();
  
  // Update all subsystems
  drum1.update();
  drum2.update();
  inputs.update();
  menu.update(currentTime);
  
  // Handle drum 1 triggers
  if (drum1.wasTriggered()) {
    drum1HitTime = currentTime;
    audio.playDrum(1, drum1.getPeakValue());
    drum1.clearTriggered();
  }
  
  // Handle drum 2 triggers
  if (drum2.wasTriggered()) {
    drum2HitTime = currentTime;
    audio.playDrum(2, drum2.getPeakValue());
    drum2.clearTriggered();
  }

  // Handle button presses
  int buttonPressed = inputs.getButtonPressed();
  if (buttonPressed != -1) {
    menu.handleButtonPress(buttonPressed);
    inputs.clearButtonPressed();
    
    // Update display state based on menu state
    if (menu.isMenuActive()) {
      displayState = STATE_MENU;
      
      // Update audio manager with new notes whenever they change
      audio.setDrum1Note(menu.getDrum1Note());
      audio.setDrum2Note(menu.getDrum2Note());
    } else {
      displayState = STATE_IDLE;
    }
  }
  
  // Check if menu timed out
  if (displayState == STATE_MENU && !menu.isMenuActive()) {
    displayState = STATE_IDLE;
  }

  // Handle pot 3 volume control with noise filtering
  int currentPot3 = inputs.getPot3Value();
  int pot3Change = abs(currentPot3 - lastPot3ForVolume);
  
  if (pot3Change > 50) {  // Increased threshold for overlay trigger
    if (pot3Initialized) {  // Only show overlay after first real change
      float volume = map(currentPot3, 0, 4095, 0, 100) / 100.0;
      audio.setVolume(volume);
      
      // Show volume overlay if not in menu
      if (displayState != STATE_MENU) {
        displayState = STATE_VOLUME_OVERLAY;
        overlayStartTime = currentTime;
        int volumePercent = (int)(volume * 100);
        display.showVolumeOverlay(volumePercent);
      }
      
      Serial.print("Volume: ");
      Serial.println(volume);
    }
    
    lastPot3ForVolume = currentPot3;
    pot3Initialized = true;
  }

  // Handle pot 1 sensitivity for drum 1 with noise filtering
  int currentPot1 = inputs.getPot1Value();
  int pot1Change = abs(currentPot1 - lastPot1ForSensitivity);
  
  if (pot1Change > 50) {  // Increased threshold
    if (pot1Initialized) {
      int triggerValue = map(currentPot1, 0, 4095, 50, 500);
      drum1.setTriggerValue(triggerValue);
      
      // Show sensitivity overlay if not in menu
      if (displayState != STATE_MENU) {
        displayState = STATE_SENSITIVITY_OVERLAY;
        overlayStartTime = currentTime;
        overlayDrumIndex = 0;
        display.showSensitivityOverlay(0, triggerValue);
      }
      
      Serial.print("Drum 1 Trigger: ");
      Serial.println(triggerValue);
    }
    
    lastPot1ForSensitivity = currentPot1;
    pot1Initialized = true;
  }
  
  // Handle pot 2 sensitivity for drum 2 with noise filtering
  int currentPot2 = inputs.getPot2Value();
  int pot2Change = abs(currentPot2 - lastPot2ForSensitivity);
  
  if (pot2Change > 50) {  // Increased threshold
    if (pot2Initialized) {
      int triggerValue = map(currentPot2, 0, 4095, 50, 500);
      drum2.setTriggerValue(triggerValue);
      
      // Show sensitivity overlay if not in menu
      if (displayState != STATE_MENU) {
        displayState = STATE_SENSITIVITY_OVERLAY;
        overlayStartTime = currentTime;
        overlayDrumIndex = 1;
        display.showSensitivityOverlay(1, triggerValue);
      }
      
      Serial.print("Drum 2 Trigger: ");
      Serial.println(triggerValue);
    }
    
    lastPot2ForSensitivity = currentPot2;
    pot2Initialized = true;
  }
  
  // Handle delayed EEPROM writes
  eepromManager.update(currentTime, menu.areNotesDirty(), 
                      menu.getLastNoteChange(),
                      menu.getDrum1Note(), menu.getDrum2Note());
  
  // Update display (handles state transitions and hit dots)
  if (currentTime - lastDisplayUpdate > 50) {  // Update at ~20Hz
    updateDisplay();
    lastDisplayUpdate = currentTime;
  }
  
  delayMicroseconds(100);
}