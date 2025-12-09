#include "display_manager.h"
#include <Wire.h>

DisplayManager::DisplayManager() 
  : display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17) {
}

void DisplayManager::begin() {
  // Initialize I2C bus 1
  Wire1.begin();
  
  // Initialize display
  display.begin();
}

void DisplayManager::showSplash() {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  display.drawStr(0, 10, "Drum Brain Test");
  display.drawStr(0, 25, "2 Drums + Audio");
  display.drawStr(0, 40, "5 Buttons");
  display.drawStr(0, 55, "Ready!");
  display.sendBuffer();
}

void DisplayManager::showDrumHit(int drumNum, int peakValue) {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  
  if (drumNum == 1) {
    display.drawStr(0, 20, "DRUM 1");
  } else {
    display.drawStr(0, 20, "DRUM 2");
  }
  
  display.setCursor(0, 50);
  display.print("Peak: ");
  display.print(peakValue);
  display.sendBuffer();
}

void DisplayManager::showButton(int buttonPin) {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(0, 20, "BUTTON");
  display.setCursor(0, 50);
  display.print("Pin: ");
  display.print(buttonPin);
  display.sendBuffer();
}
