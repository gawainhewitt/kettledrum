#include <Arduino.h>

const int DRUM_PIN = A3;
const int THRESHOLD = 50;
const int SCAN_TIME = 5;  // milliseconds to scan for peak after threshold crossed
const int MASK_TIME = 50; // milliseconds to ignore retriggering after a hit

unsigned long lastHitTime = 0;
bool scanning = false;
unsigned long scanStartTime = 0;
int peakValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(DRUM_PIN, INPUT);
  analogReadResolution(12);
  
  Serial.println("Drum trigger test - Peak detection");
  Serial.println("Hit the drum...");
}

void loop() {
  unsigned long currentTime = millis();
  int value = analogRead(DRUM_PIN);
  
  // Check if we're in mask time (ignoring retriggering)
  if (currentTime - lastHitTime < MASK_TIME) {
    delayMicroseconds(100);
    return;
  }
  
  // Detect hit start
  if (!scanning && value > THRESHOLD) {
    scanning = true;
    scanStartTime = currentTime;
    peakValue = value;
  }
  
  // During scan time, track peak
  if (scanning) {
    if (value > peakValue) {
      peakValue = value;
    }
    
    // End of scan window
    if (currentTime - scanStartTime >= SCAN_TIME) {
      Serial.print("HIT! Peak: ");
      Serial.println(peakValue);
      
      scanning = false;
      lastHitTime = currentTime;
      peakValue = 0;
    }
  }
  
  delayMicroseconds(100);
}