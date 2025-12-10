#ifndef DRUM_TRIGGER_H
#define DRUM_TRIGGER_H

#include <Arduino.h>

class DrumTrigger {
public:
  DrumTrigger(int pin, int drumNumber);
  void begin();
  void update();
  int getPeakValue() const { return peakValue; }
  bool wasTriggered() const { return triggered; }
  void clearTriggered() { triggered = false; }
  void setTriggerValue(int value);
  int getDrumNumber() const { return drumNum; }

private:
  int drumPin;
  int drumNum;
  int triggerValue;
  unsigned long lastHitTime;
  bool scanning;
  unsigned long scanStartTime;
  int peakValue;
  bool triggered;
};

#endif // DRUM_TRIGGER_H
