/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PulsarWorld.h"

#include <memory>
//==============================================================================
/**
*/

class PulsarAudioProcessorEditor  : public AudioProcessorEditor, Timer, private MidiInputCallback, private AsyncUpdater
{
public:
    PulsarAudioProcessorEditor (PulsarAudioProcessor&);
    ~PulsarAudioProcessorEditor();

    //==============================================================================

    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override
    {
        
    }
    
    // juce::MidiInputCallback
    void handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message) override;
    
    // juce::AsyncUpdater
    void handleAsyncUpdate() override;

private:
    void setMidiInput(const String& identifier);
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PulsarAudioProcessor& processor;
    
    PulsarWorld mWorld {*this, { 0.0f, 0.0f, 4.0f, 4.0f }, {0.0f, 10.0f}};
    
    CriticalSection mMidiMonitorLock;
    Array<MidiMessage> mIncomingMessages;
    
    AudioDeviceManager mDeviceManager;
    Label mMidiInputLabel    { "Midi Input Label",  "MIDI Input:" };
    ComboBox mMidiInputList;
    int mLastInputIndex = 0;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsarAudioProcessorEditor)
};
