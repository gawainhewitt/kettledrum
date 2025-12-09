#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <bus1_U8g2lib.h>

class DisplayManager {
public:
  DisplayManager();
  void begin();
  void showSplash();
  void showDrumHit(int drumNum, int peakValue);
  void showButton(int buttonPin);

private:
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C display;
};

#endif // DISPLAY_MANAGER_H
