#include "audio_manager.h"
#include "config.h"
#include "simpletimp_samples.h"

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
  patchCord1 = new AudioConnection(wavetable1, 0, mixer1, 0);
  patchCord2 = new AudioConnection(wavetable2, 0, mixer1, 1);
  patchCord3 = new AudioConnection(mixer1, 0, i2s1, 0); // Left
  patchCord4 = new AudioConnection(mixer1, 0, i2s1, 1); // Right
  
  // Setup mixer gains
  mixer1.gain(0, 0.5); // Drum 1
  mixer1.gain(1, 0.5); // Drum 2
  mixer1.gain(2, 0);
  mixer1.gain(3, 0);
  
  // Load timpani instrument into both wavetables
  wavetable1.setInstrument(simpletimp);
  wavetable2.setInstrument(simpletimp);
  
  // Set initial amplitude
  wavetable1.amplitude(1.0);
  wavetable2.amplitude(1.0);
}

void AudioManager::playDrum(int drumNum, int peakValue) {
  // Map peak value to MIDI velocity (0-127)
  int velocity = map(peakValue, TRIGGER_VALUE, 4095, 40, 127);
  velocity = constrain(velocity, 40, 127);
  
  // MIDI note 67 = G3 (the note this sample was recorded at)
  // You could assign different notes to each drum if desired
  int midiNote = 67;
  
  if (drumNum == 1) {
    wavetable1.playNote(67, velocity);
  } else {
    wavetable2.playNote(60, velocity);
  }
}

void AudioManager::setVolume(float volume) {
  volume = constrain(volume, 0.0, 1.0);
  
  if (volume < 0.01) {
    // Mute by setting mixer gains to 0
    mixer1.gain(0, 0);
    mixer1.gain(1, 0);
  } else {
    // Apply volume via mixer gains
    mixer1.gain(0, volume * 0.7);
    mixer1.gain(1, volume * 0.7);
  }
}