#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Audio.h>

class AudioManager {
public:
  AudioManager();
  void begin();
  void playDrum(int drumNum, int peakValue);

private:
  AudioSynthWaveformSine sine1;
  AudioSynthWaveformSine sine2;
  AudioMixer4 mixer1;
  AudioOutputI2S i2s1;
  AudioConnection* patchCord1;
  AudioConnection* patchCord2;
  AudioConnection* patchCord3;
  AudioConnection* patchCord4;
  AudioControlSGTL5000 sgtl5000_1;
};

#endif // AUDIO_MANAGER_H
