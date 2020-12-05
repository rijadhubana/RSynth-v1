#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;


//==============================================================================
/*
*/
class Envelope  : public juce::Component
{
public:
    Envelope(SynthFrameworkAudioProcessor&);
    ~Envelope() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SynthFrameworkAudioProcessor& audioProcessor;
    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
