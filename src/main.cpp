#include <Arduino.h>
#include "config.h"
#include "drum_trigger.h"
#include "audio_manager.h"
#include "display_manager.h"
#include "input_controls.h"

// Create instances
DrumTrigger drum1(DRUM_PIN_1, 1);
DrumTrigger drum2(DRUM_PIN_2, 2);
AudioManager audio;
DisplayManager display;
InputControls inputs;
int lastPot3ForVolume = -1;
int lastPot1ForSensitivity = -1;
int lastPot2ForSensitivity = -1;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  
  // Initialize all subsystems
  drum1.begin();
  drum2.begin();
  audio.begin();
  display.begin();
  inputs.begin();
  
  // Show splash screen
  display.showSplash();
  
  Serial.println("Drum trigger test - with audio!");
  Serial.println("Hit the drums...");
  
  delay(2000);
}

void loop() {
  // Update all subsystems
  drum1.update();
  drum2.update();
  inputs.update();
  
  // Handle drum 1 triggers
  if (drum1.wasTriggered()) {
    audio.playDrum(1, drum1.getPeakValue());
    display.showDrumHit(1, drum1.getPeakValue());
    drum1.clearTriggered();
  }
  
  // Handle drum 2 triggers
  if (drum2.wasTriggered()) {
    audio.playDrum(2, drum2.getPeakValue());
    display.showDrumHit(2, drum2.getPeakValue());
    drum2.clearTriggered();
  }

  // Handle pot 3 volume control (reverse mapped)
  int currentPot3 = inputs.getPot3Value();
  if (abs(currentPot3 - lastPot3ForVolume) > 20) {  // 20 = noise threshold
    // Reverse map: pot at 0 = volume 1.0, pot at 4095 = volume 0.1
    float volume = map(currentPot3, 0, 4095, 0, 100) / 100.0;
    audio.setVolume(volume);
    lastPot3ForVolume = currentPot3;
    
    Serial.print("Volume: ");
    Serial.println(volume);
  }

  // Handle pot 1 sensitivity for drum 1 (clockwise = more sensitive)
  int currentPot1 = inputs.getPot1Value();
  if (abs(currentPot1 - lastPot1ForSensitivity) > 20) {
    // Reverse map: pot at 0 = trigger 500, pot at 4095 = trigger 50
    int triggerValue = map(currentPot1, 0, 4095, 50, 500);
    drum1.setTriggerValue(triggerValue);
    lastPot1ForSensitivity = currentPot1;
    
    Serial.print("Drum 1 Trigger: ");
    Serial.println(triggerValue);
  }
  
  // Handle pot 2 sensitivity for drum 2 (clockwise = more sensitive)
  int currentPot2 = inputs.getPot2Value();
  if (abs(currentPot2 - lastPot2ForSensitivity) > 20) {
    // Reverse map: pot at 0 = trigger 500, pot at 4095 = trigger 50
    int triggerValue = map(currentPot2, 0, 4095, 50, 500);
    drum2.setTriggerValue(triggerValue);
    lastPot2ForSensitivity = currentPot2;
    
    Serial.print("Drum 2 Trigger: ");
    Serial.println(triggerValue);
  }
  
  // Handle button presses
  if (inputs.getButtonPressed() != -1) {
    display.showButton(inputs.getButtonPressed());
    inputs.clearButtonPressed();
  }
  
  delayMicroseconds(100);
}
