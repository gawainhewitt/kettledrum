#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <bus1_U8g2lib.h>

enum DisplayMode {
    DISPLAY_IDLE,
    DISPLAY_VOLUME_OVERLAY,
    DISPLAY_SENSITIVITY_OVERLAY,
    DISPLAY_MENU
};


class DisplayManager {
public:
  DisplayManager();
  void begin();
  void showSplash();
  void showDrumHit(int drumNum, int peakValue);
  void showButton(int buttonPin);
  void setDisplayMode(DisplayMode mode);
  void showIdleScreen(bool drum1Hit, bool drum2Hit);  
  void showVolumeOverlay(int volume);
  void showSensitivityOverlay(int drumIndex, int sensitivity);
  void showMenu(int selectedDrum, uint8_t drum1Note, uint8_t drum2Note, bool drum1Hit, bool drum2Hit);  
  void showHitDot(int drumIndex, bool state);
  void update();  // Call every loop to handle animations/timeouts

private:
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C display;
  DisplayMode currentMode;
  unsigned long lastUpdateTime;
};

#endif // DISPLAY_MANAGER_H
