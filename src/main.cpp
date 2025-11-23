#include <Arduino.h>
#include <bus1_U8g2lib.h>
#include <Wire.h>
#include <Audio.h>

const int DRUM_PIN_1 = A0;
const int DRUM_PIN_2 = A1;
const int THRESHOLD = 50;
const int TRIGGER_VALUE = 200; // Trigger sound at this peak value
const int SCAN_TIME = 5;
const int MASK_TIME = 50;

// Potentiometer pins
const int POT_PIN_1 = A10;
const int POT_PIN_2 = A11;
const int POT_PIN_3 = A12;

// Tact switch pins
const int BUTTON_PINS[] = {2, 3, 4, 5, 9};
const int NUM_BUTTONS = 5;
bool lastButtonState[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH};

// State for drum 1
unsigned long lastHitTime1 = 0;
bool scanning1 = false;
unsigned long scanStartTime1 = 0;
int peakValue1 = 0;

// State for drum 2
unsigned long lastHitTime2 = 0;
bool scanning2 = false;
unsigned long scanStartTime2 = 0;
int peakValue2 = 0;

// Pot values
int pot1Value = 0;
int pot2Value = 0;
int pot3Value = 0;
int lastPot1Value = 0;
int lastPot2Value = 0;
int lastPot3Value = 0;
unsigned long lastPotRead = 0;
const int POT_READ_INTERVAL = 100;
const int POT_CHANGE_THRESHOLD = 15;

// OLED display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);

// Audio setup - use mixer to combine both drums
AudioSynthWaveformSine sine1;
AudioSynthWaveformSine sine2;
AudioMixer4 mixer1;
AudioOutputI2S i2s1;
AudioConnection patchCord1(sine1, 0, mixer1, 0);
AudioConnection patchCord2(sine2, 0, mixer1, 1);
AudioConnection patchCord3(mixer1, 0, i2s1, 0); // Left
AudioConnection patchCord4(mixer1, 0, i2s1, 1); // Right
AudioControlSGTL5000 sgtl5000_1;

void setup() {
  Serial.begin(115200);
  pinMode(DRUM_PIN_1, INPUT);
  pinMode(DRUM_PIN_2, INPUT);
  pinMode(POT_PIN_1, INPUT);
  pinMode(POT_PIN_2, INPUT);
  pinMode(POT_PIN_3, INPUT);
  analogReadResolution(12);
  
  // Setup button pins
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  
  // Initialize audio
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  
  // Setup mixer gains
  mixer1.gain(0, 0.5); // Drum 1
  mixer1.gain(1, 0.5); // Drum 2
  mixer1.gain(2, 0);
  mixer1.gain(3, 0);
  
  sine1.frequency(440); // Drum 1 sound - 440Hz
  sine1.amplitude(0);
  sine2.frequency(550); // Drum 2 sound - 550Hz
  sine2.amplitude(0);
  
  // Initialize I2C bus 1
  Wire1.begin();
  
  // Initialize display
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  display.drawStr(0, 10, "Drum Brain Test");
  display.drawStr(0, 25, "2 Drums + Audio");
  display.drawStr(0, 40, "5 Buttons");
  display.drawStr(0, 55, "Ready!");
  display.sendBuffer();
  
  Serial.println("Drum trigger test - with audio!");
  Serial.println("Hit the drums...");
  
  // Read initial pot values
  pot1Value = analogRead(POT_PIN_1);
  pot2Value = analogRead(POT_PIN_2);
  pot3Value = analogRead(POT_PIN_3);
  lastPot1Value = pot1Value;
  lastPot2Value = pot2Value;
  lastPot3Value = pot3Value;
  
  delay(2000);
}

void playDrumSound(int drumNum, int peak) {
  // Map peak value to amplitude (0.0 to 1.0)
  float amplitude = map(peak, TRIGGER_VALUE, 4095, 100, 1000) / 1000.0;
  amplitude = constrain(amplitude, 0.1, 1.0);
  
  if (drumNum == 1) {
    sine1.amplitude(amplitude);
    delay(50); // Sound duration
    sine1.amplitude(0);
  } else {
    sine2.amplitude(amplitude);
    delay(50); // Sound duration
    sine2.amplitude(0);
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  // Read both drums
  int value1 = analogRead(DRUM_PIN_1);
  int value2 = analogRead(DRUM_PIN_2);
  
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
  
  // Process drum 1
  if (currentTime - lastHitTime1 >= MASK_TIME) {
    if (!scanning1 && value1 > THRESHOLD) {
      scanning1 = true;
      scanStartTime1 = currentTime;
      peakValue1 = value1;
    }
    
    if (scanning1) {
      if (value1 > peakValue1) {
        peakValue1 = value1;
      }
      
      if (currentTime - scanStartTime1 >= SCAN_TIME) {
        Serial.print("DRUM 1 HIT! Peak: ");
        Serial.println(peakValue1);
        
        // Play sound if above trigger value
        if (peakValue1 >= TRIGGER_VALUE) {
          playDrumSound(1, peakValue1);
        }
        
        // Update display
        display.clearBuffer();
        display.setFont(u8g2_font_ncenB14_tr);
        display.drawStr(0, 20, "DRUM 1");
        display.setCursor(0, 50);
        display.print("Peak: ");
        display.print(peakValue1);
        display.sendBuffer();
        
        scanning1 = false;
        lastHitTime1 = currentTime;
        peakValue1 = 0;
      }
    }
  }
  
  // Process drum 2
  if (currentTime - lastHitTime2 >= MASK_TIME) {
    if (!scanning2 && value2 > THRESHOLD) {
      scanning2 = true;
      scanStartTime2 = currentTime;
      peakValue2 = value2;
    }
    
    if (scanning2) {
      if (value2 > peakValue2) {
        peakValue2 = value2;
      }
      
      if (currentTime - scanStartTime2 >= SCAN_TIME) {
        Serial.print("DRUM 2 HIT! Peak: ");
        Serial.println(peakValue2);
        
        // Play sound if above trigger value
        if (peakValue2 >= TRIGGER_VALUE) {
          playDrumSound(2, peakValue2);
        }
        
        // Update display
        display.clearBuffer();
        display.setFont(u8g2_font_ncenB14_tr);
        display.drawStr(0, 20, "DRUM 2");
        display.setCursor(0, 50);
        display.print("Peak: ");
        display.print(peakValue2);
        display.sendBuffer();
        
        scanning2 = false;
        lastHitTime2 = currentTime;
        peakValue2 = 0;
      }
    }
  }
  
  // Check buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool currentState = digitalRead(BUTTON_PINS[i]);
    
    if (currentState == LOW && lastButtonState[i] == HIGH) {
      Serial.print("BUTTON ");
      Serial.print(BUTTON_PINS[i]);
      Serial.println(" PRESSED");
      
      display.clearBuffer();
      display.setFont(u8g2_font_ncenB14_tr);
      display.drawStr(0, 20, "BUTTON");
      display.setCursor(0, 50);
      display.print("Pin: ");
      display.print(BUTTON_PINS[i]);
      display.sendBuffer();
    }
    
    lastButtonState[i] = currentState;
  }
  
  delayMicroseconds(100);
}