#include "display_manager.h"
#include <Wire.h>
#include "config.h"

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
  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(20, 35, "OrchLab");
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

void DisplayManager::setDisplayMode(DisplayMode mode) {
    currentMode = mode;
    lastUpdateTime = millis();
}

void DisplayManager::showIdleScreen(bool drum1Hit, bool drum2Hit) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB14_tr);
    display.drawStr(20, 35, "OrchLab");
    
    // Draw hit dots at bottom - filled when active, empty when not
    if (drum1Hit) {
        display.drawDisc(10, 55, 3);  // Filled circle
    } else {
        display.drawCircle(10, 55, 3);  // Empty circle
    }
    
    if (drum2Hit) {
        display.drawDisc(118, 55, 3);  // Filled circle
    } else {
        display.drawCircle(118, 55, 3);  // Empty circle
    }
    
    display.sendBuffer();
}

void DisplayManager::showVolumeOverlay(int volume) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB10_tr);
    display.drawStr(10, 25, "Volume:");
    display.setCursor(10, 45);
    display.print(volume);
    display.sendBuffer();
}

void DisplayManager::showSensitivityOverlay(int drumIndex, int sensitivity) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB10_tr);
    String label = "Drum " + String(drumIndex + 1) + " Sens:";
    display.drawStr(10, 25, label.c_str());
    display.setCursor(10, 45);
    display.print(sensitivity);
    display.sendBuffer();
}

void DisplayManager::showMenu(int selectedDrum, uint8_t drum1Note, uint8_t drum2Note, bool drum1Hit, bool drum2Hit) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB10_tr);
    
    // Drum 1 line
    String drum1Text = "Drum 1: " + midiToNoteName(drum1Note);
    if (selectedDrum == 0) {
        display.drawStr(0, 20, ">");
    }
    display.drawStr(15, 20, drum1Text.c_str());
    
    // Drum 2 line
    String drum2Text = "Drum 2: " + midiToNoteName(drum2Note);
    if (selectedDrum == 1) {
        display.drawStr(0, 40, ">");
    }
    display.drawStr(15, 40, drum2Text.c_str());
    
    // Hit dots at bottom - filled when active, empty when not
    if (drum1Hit) {
        display.drawDisc(10, 55, 3);
    } else {
        display.drawCircle(10, 55, 3);
    }
    
    if (drum2Hit) {
        display.drawDisc(118, 55, 3);
    } else {
        display.drawCircle(118, 55, 3);
    }
    
    display.sendBuffer();
}

void DisplayManager::showHitDot(int drumIndex, bool state) {
    int x = (drumIndex == 0) ? 10 : 118;
    int y = 55;
    
    if (state) {
        display.drawDisc(x, y, 3);  // Filled circle
    } else {
        display.drawCircle(x, y, 3); // Empty circle
    }
    display.sendBuffer();
}