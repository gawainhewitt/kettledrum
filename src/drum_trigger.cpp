#include "drum_trigger.h"
#include "config.h"

DrumTrigger::DrumTrigger(int pin, int drumNumber) 
  : drumPin(pin), drumNum(drumNumber), lastHitTime(0), 
    scanning(false), scanStartTime(0), peakValue(0), triggered(false),
    triggerValue(TRIGGER_VALUE) {
}

void DrumTrigger::begin() {
  pinMode(drumPin, INPUT);
}

void DrumTrigger::update() {
  unsigned long currentTime = millis();
  int value = analogRead(drumPin);
  
  // Process drum trigger
  if (currentTime - lastHitTime >= MASK_TIME) {
    if (!scanning && value > THRESHOLD) {
      scanning = true;
      scanStartTime = currentTime;
      peakValue = value;
    }
    
    if (scanning) {
      if (value > peakValue) {
        peakValue = value;
      }
      
      if (currentTime - scanStartTime >= SCAN_TIME) {
        Serial.print("DRUM ");
        Serial.print(drumNum);
        Serial.print(" HIT! Peak: ");
        Serial.println(peakValue);
        
        if (peakValue >= triggerValue) {
          triggered = true;
        }
        
        scanning = false;
        lastHitTime = currentTime;
      }
    }
  }
}

void DrumTrigger::setTriggerValue(int value) {
  triggerValue = constrain(value, 10, 1000);
}
