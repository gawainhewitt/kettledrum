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
  
  // Handle button presses
  if (inputs.getButtonPressed() != -1) {
    display.showButton(inputs.getButtonPressed());
    inputs.clearButtonPressed();
  }
  
  delayMicroseconds(100);
}
