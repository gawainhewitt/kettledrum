#include "audio_manager.h"
#include "config.h"

AudioManager::AudioManager() 
  : patchCord1(nullptr), patchCord2(nullptr), 
    patchCord3(nullptr), patchCord4(nullptr) {
}

void AudioManager::begin() {
  // Initialize audio
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  
  // Create audio connections
  patchCord1 = new AudioConnection(sine1, 0, mixer1, 0);
  patchCord2 = new AudioConnection(sine2, 0, mixer1, 1);
  patchCord3 = new AudioConnection(mixer1, 0, i2s1, 0); // Left
  patchCord4 = new AudioConnection(mixer1, 0, i2s1, 1); // Right
  
  // Setup mixer gains
  mixer1.gain(0, 0.5); // Drum 1
  mixer1.gain(1, 0.5); // Drum 2
  mixer1.gain(2, 0);
  mixer1.gain(3, 0);
  
  // Setup initial frequencies
  sine1.frequency(440); // Drum 1 sound - 440Hz
  sine1.amplitude(0);
  sine2.frequency(550); // Drum 2 sound - 550Hz
  sine2.amplitude(0);
}

void AudioManager::playDrum(int drumNum, int peakValue) {
  // Map peak value to amplitude (0.0 to 1.0)
  float amplitude = map(peakValue, TRIGGER_VALUE, 4095, 100, 1000) / 1000.0;
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
