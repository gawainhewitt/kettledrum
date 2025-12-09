#include "input_controls.h"
#include "config.h"

InputControls::InputControls() 
  : pot1Value(0), pot2Value(0), pot3Value(0),
    lastPot1Value(0), lastPot2Value(0), lastPot3Value(0),
    lastPotRead(0), buttonPressed(-1) {
  
  for (int i = 0; i < NUM_BUTTONS; i++) {
    lastButtonState[i] = HIGH;
  }
}

void InputControls::begin() {
  // Setup pot pins
  pinMode(POT_PIN_1, INPUT);
  pinMode(POT_PIN_2, INPUT);
  pinMode(POT_PIN_3, INPUT);
  
  // Setup button pins
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  
  // Read initial pot values
  pot1Value = analogRead(POT_PIN_1);
  pot2Value = analogRead(POT_PIN_2);
  pot3Value = analogRead(POT_PIN_3);
  lastPot1Value = pot1Value;
  lastPot2Value = pot2Value;
  lastPot3Value = pot3Value;
}

void InputControls::update() {
  unsigned long currentTime = millis();
  
  // Read pots periodically and check for changes
  if (currentTime - lastPotRead >= POT_READ_INTERVAL) {
    pot1Value = analogRead(POT_PIN_1);
    pot2Value = analogRead(POT_PIN_2);
    pot3Value = analogRead(POT_PIN_3);
    lastPotRead = currentTime;
    
    // Only print if any pot changed significantly
    if (abs(pot1Value - lastPot1Value) > POT_CHANGE_THRESHOLD ||
        abs(pot2Value - lastPot2Value) > POT_CHANGE_THRESHOLD ||
        abs(pot3Value - lastPot3Value) > POT_CHANGE_THRESHOLD) {
      
      Serial.print("Pot1: ");
      Serial.print(pot1Value);
      Serial.print(" | Pot2: ");
      Serial.print(pot2Value);
      Serial.print(" | Pot3: ");
      Serial.println(pot3Value);
      
      lastPot1Value = pot1Value;
      lastPot2Value = pot2Value;
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
