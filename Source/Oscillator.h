#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Oscillator  : public juce::Component, private juce::ComboBox::Listener
{
public:
    Oscillator(SynthFrameworkAudioProcessor&);
    ~Oscillator() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox* box) override;

private:
    juce::ComboBox oscMenu;
    SynthFrameworkAudioProcessor& audioProcessor;
    juce::ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> waveFormAttachment;
    juce::Slider pulseRectDuty;
    juce::ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> dutyAttachment;
    juce::Slider volumeSlider;
    juce::ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    juce::Label waveformtypeLabel;
    juce::Label volumeLabel;
    juce::Label pulseDutyLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};
