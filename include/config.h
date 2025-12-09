#ifndef CONFIG_H
#define CONFIG_H

// Drum trigger pins
const int DRUM_PIN_1 = A0;
const int DRUM_PIN_2 = A1;

// Trigger parameters
const int THRESHOLD = 50;
const int TRIGGER_VALUE = 100;
const int SCAN_TIME = 5;
const int MASK_TIME = 50;

// Potentiometer pins
const int POT_PIN_1 = A10;
const int POT_PIN_2 = A11;
const int POT_PIN_3 = A12;
const int POT_READ_INTERVAL = 100;
const int POT_CHANGE_THRESHOLD = 15;

// Tact switch pins
const int BUTTON_PINS[] = {2, 3, 4, 5, 9};
const int NUM_BUTTONS = 5;

#endif // CONFIG_H
