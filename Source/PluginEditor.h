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
    
    void sendNoteOnMessage(int noteNumber, float velocity);

private:
    void setMidiInput(const String& identifier);
    void setMidiOutput(const String& identifier);
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PulsarAudioProcessor& processor;
    
    Physics::PulsarWorld mWorld {*this, { 0.0f, 0.0f, 4.0f, 4.0f }, {0.0f, 10.0f}};
    
    CriticalSection mMidiMonitorLock;
    Array<MidiMessage> mIncomingMessages;
    
    AudioDeviceManager mDeviceManager;
    Label mMidiInputLabel    { "Midi Input Label",  "MIDI Input / Channel:" };
    Label mMidiOutputLabel { "Midi Output Label", "Midi Output / Channel:" };
    ComboBox mMidiInputList, mMidiOutputList;
    ComboBox mMidiInputChannelList, mMidiOutputChannelList;
    
    std::unique_ptr<MidiOutput> mMidiOutput;
    int mMidiInputChannel;
    int mMidiOutputChannel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsarAudioProcessorEditor)
};
