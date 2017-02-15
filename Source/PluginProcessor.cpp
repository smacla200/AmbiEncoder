/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AmbiEncoderAudioProcessor::AmbiEncoderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput ("Input", AudioChannelSet::mono(), true)
#endif
                  .withOutput ("Output", AudioChannelSet::ambisonic(), true)
#endif
                  )
#endif
{
}

AmbiEncoderAudioProcessor::~AmbiEncoderAudioProcessor()
{
}

//==============================================================================
const String AmbiEncoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmbiEncoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmbiEncoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double AmbiEncoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmbiEncoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AmbiEncoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmbiEncoderAudioProcessor::setCurrentProgram (int index)
{
}

const String AmbiEncoderAudioProcessor::getProgramName (int index)
{
    return String();
}

void AmbiEncoderAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AmbiEncoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AmbiEncoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmbiEncoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::ambisonic())
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

void AmbiEncoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&
                                              midiMessages)
{
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    // Convert pan position in degrees to radians
    float azimuth = (M_PI/180.f) * panPosition;
    
    // Get a pointer to each of the Ambisonic channels
    float* channelDataW = buffer.getWritePointer (0);
    float* channelDataX = buffer.getWritePointer (1);
    float* channelDataY = buffer.getWritePointer (2);
    float* channelDataZ = buffer.getWritePointer (3); // Not used
    
    // Loop through each sample
    for (int i = 0; i < numSamples; i++)
    {
        float audioIn = channelDataW[i]; // Store input in temp variable
        
        // Do the encoding (horizontal only)
        channelDataW[i] = audioIn * 0.707;
        channelDataX[i] = audioIn * cos(azimuth);
        channelDataY[i] = audioIn * sin(azimuth);
        
    }
}

//==============================================================================
bool AmbiEncoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AmbiEncoderAudioProcessor::createEditor()
{
    return new AmbiEncoderAudioProcessorEditor (*this);
}

//==============================================================================
void AmbiEncoderAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AmbiEncoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbiEncoderAudioProcessor();
}
