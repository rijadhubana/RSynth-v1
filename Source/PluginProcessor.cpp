#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthFrameworkAudioProcessor::SynthFrameworkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),tree (*this,nullptr)
#endif
{
    mySynth.clearVoices();
    for (int i = 0; i < 8; i++)
    {
        mySynth.addVoice(new SynthVoice());
    }
    mySynth.clearSounds();
    mySynth.addSound(new SynthSound());
#pragma region rangesAndTreeParameters
    NormalisableRange<float> attackParam(0.1f, 5000.0f);
    NormalisableRange<float> releaseParam(0.1f, 5000.0f);
    NormalisableRange<float> decayParam(1.0f, 2000.0f);
    NormalisableRange<float> sustainParam(0.0f, 1.0f);
    NormalisableRange<float> waveformParam(0, 6);
    NormalisableRange<float> pulseRectParam(0.0f, 1.0f);
    NormalisableRange<float> volumeParam(-48.0f,0.0f);

    tree.createAndAddParameter("attack", "Attack", "Attack", attackParam, 100.0f, nullptr, nullptr);
    tree.createAndAddParameter("release", "Release", "Release", releaseParam, 500.0f, nullptr, nullptr);
    tree.createAndAddParameter("waveform", "Waveform", "Waveform", waveformParam, 0, nullptr, nullptr);
    tree.createAndAddParameter("decay", "Decay", "Decay", decayParam, 200.0f, nullptr, nullptr);
    tree.createAndAddParameter("sustain", "Sustain", "Sustain", sustainParam, 0.8f, nullptr, nullptr);
    tree.createAndAddParameter("pulserectduty", "PulseRectDuty", "PulseRectDuty", pulseRectParam, 0.5, nullptr, nullptr);
    tree.createAndAddParameter("volume", "Volume", "Volume", volumeParam, -22.0f, nullptr, nullptr);
    tree.state = ValueTree("MyTree");
#pragma endregion
}

SynthFrameworkAudioProcessor::~SynthFrameworkAudioProcessor()
{
}

//==============================================================================
const juce::String SynthFrameworkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthFrameworkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthFrameworkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthFrameworkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthFrameworkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthFrameworkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthFrameworkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthFrameworkAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthFrameworkAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthFrameworkAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthFrameworkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
    mySynth.setCurrentPlaybackSampleRate(lastSampleRate);
}

void SynthFrameworkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthFrameworkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthFrameworkAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (size_t i = 0; i < mySynth.getNumVoices(); i++)
    {
        if (myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i)))
        {
            myVoice->getParam(*tree.getRawParameterValue("attack"), *tree.getRawParameterValue("release"),
                *tree.getRawParameterValue("sustain"), *tree.getRawParameterValue("decay"));
            float waveformType = *tree.getRawParameterValue("waveform");
            myVoice->getOscType(waveformType);       
            myVoice->getVolumeParam(*tree.getRawParameterValue("volume"));
            if (waveformType==3)
                myVoice->setDutyParam(*tree.getRawParameterValue("pulserectduty"));
        }
    }
    buffer.clear();
    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}
//==============================================================================
bool SynthFrameworkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthFrameworkAudioProcessor::createEditor()
{
    return new SynthFrameworkAudioProcessorEditor (*this);
}

//==============================================================================
void SynthFrameworkAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto xml = tree.state.createXml();
    copyXmlToBinary(*xml, destData);

}

void SynthFrameworkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto savedParameters = getXmlFromBinary(data, sizeInBytes);
    if (savedParameters != nullptr)
    {
        if (savedParameters->hasTagName(tree.state.getType()))
        {
            tree.state = ValueTree::fromXml(*savedParameters);
        }
    }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthFrameworkAudioProcessor();
}
