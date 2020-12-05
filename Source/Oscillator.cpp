#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator(SynthFrameworkAudioProcessor& processor):audioProcessor(processor)
{
    setSize(160, 160);
    oscMenu.addItem("Sine", 1);
    oscMenu.addItem("Saw", 2);
    oscMenu.addItem("Square", 3);
    oscMenu.addItem("Pulse", 4);
    oscMenu.addItem("Triangle", 5);
    oscMenu.addItem("Phasor", 6);
    oscMenu.addItem("Noise", 7);
    oscMenu.setJustificationType(juce::Justification::centred);
    oscMenu.addListener(this);
    addAndMakeVisible(&oscMenu);
    waveFormAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.tree, "waveform", oscMenu);
    pulseRectDuty.setSliderStyle(Slider::LinearHorizontal);
    pulseRectDuty.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    pulseRectDuty.setRange(0.0f, 1.0f);
    //pulseRectDuty.setValue(0.5f);
    pulseRectDuty.setPopupDisplayEnabled(true, true, this);
    dutyAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "pulserectduty", pulseRectDuty);
    addAndMakeVisible(&pulseRectDuty);
    pulseRectDuty.setVisible(false);
    volumeSlider.setSliderStyle(Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    volumeSlider.setRange(-48.0f,0.0f);
    //volumeSlider.setValue(-22.0f);
    volumeSlider.setPopupDisplayEnabled(true, true, this);
    volumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "volume", volumeSlider);
    addAndMakeVisible(&volumeSlider);
    waveformtypeLabel.setText("Waveform type: ", juce::NotificationType::dontSendNotification);
    pulseDutyLabel.setText("Pulse duty: ", juce::NotificationType::dontSendNotification);
    volumeLabel.setText("Master volume: ", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(&waveformtypeLabel);
    addAndMakeVisible(&pulseDutyLabel);
    pulseDutyLabel.setVisible(false);
    addAndMakeVisible(&volumeLabel);

}

Oscillator::~Oscillator()
{
}

void Oscillator::paint (juce::Graphics& g)
{
    juce::Rectangle<float> rectArea(20, 20, 140, 140);
    juce::Rectangle<float> titleArea(0, 0, 170, 20);
    g.setColour(juce::Colours::white);
    g.drawText("LFO", titleArea, juce::Justification::centred);

    g.setColour(juce::Colours::green);
    g.drawRoundedRectangle(rectArea, 15, 2);
}

void Oscillator::resized()
{
    juce::Rectangle<int> rectArea(30, 25, 120, 120);
    waveformtypeLabel.setBounds(rectArea.removeFromTop(20));
    oscMenu.setBounds(rectArea.removeFromTop(20).removeFromRight(200));
    volumeLabel.setBounds(rectArea.removeFromTop(20));
    volumeSlider.setBounds(rectArea.removeFromTop(20));
    pulseDutyLabel.setBounds(rectArea.removeFromTop(20));
    pulseRectDuty.setBounds(rectArea.removeFromTop(20));
}

void Oscillator::comboBoxChanged(juce::ComboBox* box)
{
    if (oscMenu.getSelectedId()==4)
    {
        pulseRectDuty.setVisible(true);
        pulseDutyLabel.setVisible(true);
    }
    else
    {
        pulseRectDuty.setVisible(false);
        pulseDutyLabel.setVisible(false);
        pulseRectDuty.setValue(0.5f, dontSendNotification);
    }
}
