#include "input_controls.h"
#include "config.h"

InputControls::InputControls() 
  : pot3Value(0), lastPot3Value(0), 
  lastPotRead(0), buttonPressed(-1) {
  
  for (int i = 0; i < NUM_BUTTONS; i++) {
    lastButtonState[i] = HIGH;
  }
}

void InputControls::begin() {
  // Setup pot pins
  pinMode(POT_PIN_3, INPUT);
  
  // Setup button pins
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  
  // Read initial pot values
  pot3Value = analogRead(POT_PIN_3);
  lastPot3Value = pot3Value;
}

void InputControls::update() {
  unsigned long currentTime = millis();
  
  // Read pots periodically and check for changes
  if (currentTime - lastPotRead >= POT_READ_INTERVAL) {
    pot3Value = analogRead(POT_PIN_3);
    lastPotRead = currentTime;
    
    // Only print if any pot changed significantly
    if (abs(pot3Value - lastPot3Value) > POT_CHANGE_THRESHOLD) {
      
      Serial.print(" | Pot3: ");
      Serial.println(pot3Value);
      
      lastPot3Value = pot3Value;
    }
  }
  
  // Check buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool currentState = digitalRead(BUTTON_PINS[i]);
    
    if (currentState == LOW && lastButtonState[i] == HIGH) {
      Serial.print("BUTTON ");
      Serial.print(BUTTON_PINS[i]);
      Serial.println(" PRESSED");
      
      buttonPressed = BUTTON_PINS[i];
    }
    
    lastButtonState[i] = currentState;
  }
}
