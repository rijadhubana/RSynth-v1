#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthFrameworkAudioProcessorEditor::SynthFrameworkAudioProcessorEditor (SynthFrameworkAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), oscGui(p), envGui(p)
{
    setSize(360, 180);
    addAndMakeVisible(&oscGui);
    addAndMakeVisible(&envGui);
}

SynthFrameworkAudioProcessorEditor::~SynthFrameworkAudioProcessorEditor()
{
}

//==============================================================================
void SynthFrameworkAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void SynthFrameworkAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds();
    const int componentWidth = 160;
    const int componentHeight = 160;
    oscGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    envGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
}
