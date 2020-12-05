#include <JuceHeader.h>
#include "Envelope.h"

//==============================================================================
Envelope::Envelope(SynthFrameworkAudioProcessor& p): audioProcessor(p)
{
    setSize(160, 160);
    attackSlider.setSliderStyle(Slider::LinearVertical);
    attackSlider.setRange(0.1f, 5000.0f);
    //attackSlider.setValue(100.0f);
    attackSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.tree, "attack", attackSlider);
    addAndMakeVisible(&attackSlider);
    decaySlider.setSliderStyle(Slider::LinearVertical);
    decaySlider.setRange(1.0f, 2000.0f);
   // decaySlider.setValue(200.0f);
    decaySlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    decayAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.tree, "decay", decaySlider);
    addAndMakeVisible(&decaySlider);
    sustainSlider.setSliderStyle(Slider::LinearVertical);
    sustainSlider.setRange(0.0f, 1.0f);
    //sustainSlider.setValue(0.8f);
    sustainSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    sustainAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.tree, "sustain", sustainSlider);
    addAndMakeVisible(&sustainSlider);
    releaseSlider.setSliderStyle(Slider::LinearVertical);
    releaseSlider.setRange(0.1f, 5000.0f);
    //releaseSlider.setValue(500.0f);
    releaseSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.tree, "release", releaseSlider);
    addAndMakeVisible(&releaseSlider);

    attackLabel.setText("A", juce::NotificationType::dontSendNotification);
    decayLabel.setText("D", juce::NotificationType::dontSendNotification);
    sustainLabel.setText("S", juce::NotificationType::dontSendNotification);
    releaseLabel.setText("R", juce::NotificationType::dontSendNotification);
    attackLabel.setColour(1, juce::Colours::white);
    decayLabel.setColour(1, juce::Colours::white);
    sustainLabel.setColour(1, juce::Colours::white);
    releaseLabel.setColour(1, juce::Colours::white);
    attackLabel.setJustificationType(juce::Justification::centred);
    decayLabel.setJustificationType(juce::Justification::centred);
    sustainLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(&attackLabel);
    addAndMakeVisible(&decayLabel);
    addAndMakeVisible(&sustainLabel);
    addAndMakeVisible(&releaseLabel);
}

Envelope::~Envelope()
{
}

void Envelope::paint (juce::Graphics& g)
{
    juce::Rectangle<float> rectArea(20, 20, 140, 140);
    juce::Rectangle<float> titleArea(0, 0, 170, 20);
    g.setColour(juce::Colours::white);
    g.drawText("Envelope", titleArea, juce::Justification::centred);

    g.setColour(juce::Colours::green);
    g.drawRoundedRectangle(rectArea, 15, 2);
}

void Envelope::resized()
{
    attackSlider.setBounds(25, 25, 120 / 4, 100);
    decaySlider.setBounds(25 + 120 / 4, 25, 120 / 4, 100);
    sustainSlider.setBounds(25 + 120 / 4 + 120 / 4, 25, 120 / 4, 100);
    releaseSlider.setBounds(25 + 120 / 4 + 120 / 4 + 120 / 4, 25, 120 / 4, 100);
    attackLabel.setBounds(30, 125, 20, 20);
    decayLabel.setBounds(30 + 120 / 4, 125, 20, 20);
    sustainLabel.setBounds(30 + 120 / 4 + 120 / 4, 125, 20, 20);
    releaseLabel.setBounds(30 + 120 / 4 + 120 / 4 + 120 / 4, 125, 20, 20);
}