#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Audio.h>

class AudioManager {
  public:
    AudioManager();
    void begin();
    void playDrum(int drumNum, int peakValue);
    void setVolume(float volume);
    void playDrumNote(int drumNum, int midiNote, int peakValue);   
    void setDrum1Note(uint8_t midiNote) { drum1Note = midiNote; }   
    void setDrum2Note(uint8_t midiNote) { drum2Note = midiNote; }

  private:
    AudioSynthWavetable wavetable1;
    AudioSynthWavetable wavetable2;
    AudioMixer4 mixer1;
    AudioOutputI2S i2s1;
    AudioConnection* patchCord1;
    AudioConnection* patchCord2;
    AudioConnection* patchCord3;
    AudioConnection* patchCord4;
    AudioControlSGTL5000 sgtl5000_1;
    uint8_t drum1Note;  
    uint8_t drum2Note;
};

#endif // AUDIO_MANAGER_H
