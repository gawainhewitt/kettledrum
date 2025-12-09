#ifndef INPUT_CONTROLS_H
#define INPUT_CONTROLS_H

#include <Arduino.h>

class InputControls {
public:
  InputControls();
  void begin();
  void update();
  int getPot1Value() const { return pot1Value; }
  int getPot2Value() const { return pot2Value; }
  int getPot3Value() const { return pot3Value; }
  int getButtonPressed() const { return buttonPressed; }
  void clearButtonPressed() { buttonPressed = -1; }

private:
  // Pot values
  int pot1Value;
  int pot2Value;
  int pot3Value;
  int lastPot1Value;
  int lastPot2Value;
  int lastPot3Value;
  unsigned long lastPotRead;
  
  // Button states
  bool lastButtonState[5];
  int buttonPressed;
};

#endif // INPUT_CONTROLS_H
