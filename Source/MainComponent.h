/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PulsarWorld.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent
: public Component
, public KeyListener
, private MidiInputCallback
, private AsyncUpdater
, private juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    // juce::keyPressed
    bool keyPressed (const KeyPress &key, Component *originatingComponent) override;
       
    void mouseUp (const MouseEvent& event) override;
       
    // juce::MidiInputCallback
    void handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message) override;
       
    // juce::AsyncUpdater
    void handleAsyncUpdate() override;
       
    void sendNoteOnMessage(int noteNumber, float velocity);

private:
    //==============================================================================
    // Your private member variables go here...
    // juce::changeListener
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
   
    void setMidiInput(const String& identifier);
    void setMidiOutput(const String& identifier);
   
    Physics::PulsarWorld mWorld {*this, { 0.0f, 0.0f, 4.0f, 4.0f }, {0.0f, 10.0f}};
   
    CriticalSection mMidiMonitorLock;
    Array<MidiMessage> mIncomingMessages;
   
    AudioDeviceManager mDeviceManager;
    Label mMidiChannelLabel {"MIDI CH (in/out)"};
    ComboBox mMidiInputChannelList, mMidiOutputChannelList;
   
    juce::Slider mRotationSlider;
    juce::Slider mGravitySlider;
   
    std::unique_ptr<MidiOutput> mMidiOutput;
    int mMidiInputChannel;
    int mMidiOutputChannel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
