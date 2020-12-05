#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "maximilian.h"

using namespace juce;

class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound)
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    void getParam(float attack,float release,float sustainValue, float decayValue)
    {
        envelope1.setAttack(double(attack));
        envelope1.setRelease(double(release));
        envelope1.setDecay(double(decayValue));
        envelope1.setSustain(double(sustainValue));
    }
    void getVolumeParam(float volumeValue)
    {
        _volumeValue = pow(10, volumeValue / 20);
    }
    double setEnvelope()
    {
        return envelope1.adsr(setOscType(), envelope1.trigger) * level;
    }
    void getOscType(float selection)
    {
        waveformType = selection;
    }
    void setDutyParam(float dutyValue)
    {
        _pulseRectangleDuty = dutyValue;
    }
    double setOscType()
    {
        if (waveformType == 0)
            return oscillator1.sinewave(frequency);
        else if (waveformType == 1)
            return oscillator1.saw(frequency);
        else if (waveformType == 2)
            return oscillator1.square(frequency);
        else if (waveformType == 3)
            return oscillator1.pulse(frequency,_pulseRectangleDuty);
        else if (waveformType == 4)
            return oscillator1.triangle(frequency);
        else if (waveformType == 5)
            return oscillator1.phasor(frequency);
        else if (waveformType == 6)
            return oscillator1.noise();
        else return oscillator1.sinewave(frequency);
    }
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
    {
        envelope1.trigger = 1;
        level = velocity;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }
    void stopNote(float velocity, bool allowTailOff)
    {
        envelope1.trigger = 0;
        allowTailOff = true;
        if (velocity==0)
            clearCurrentNote();
    }
    void pitchWheelMoved(int newPitchWheelValue)
    {

    }
    void controllerMoved(int controllerNumber, int newControllerValue)
    {

    }
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, setEnvelope() * _volumeValue);
            }
            ++startSample;
        }
    }
private:
    double level;
    double frequency;
    int waveformType;
    maxiOsc oscillator1;
    maxiEnv envelope1;
    float _pulseRectangleDuty;
    float _volumeValue;
};