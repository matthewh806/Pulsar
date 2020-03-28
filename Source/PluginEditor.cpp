/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils.h"

//==============================================================================

PulsarAudioProcessorEditor::PulsarAudioProcessorEditor (PulsarAudioProcessor& p)
: AudioProcessorEditor (&p)
, processor (p)
{
    setSize (400, 400);
    
    addKeyListener(this);
    
    addAndMakeVisible(mMidiInputLabel);
    addAndMakeVisible(mMidiInputList);
    addAndMakeVisible(mMidiInputChannelList);
    
    addAndMakeVisible(mMidiOutputLabel);
    addAndMakeVisible(mMidiOutputList);
    addAndMakeVisible(mMidiOutputChannelList);
    
    mMidiInputList.setTextWhenNoChoicesAvailable("No Midi Inputs Enabled");
    auto midiInputs = MidiInput::getAvailableDevices();
    mMidiInputList.onChange = [this] {
        auto const index = mMidiInputList.getSelectedItemIndex();
        auto const midiInputs = MidiInput::getAvailableDevices();
        setMidiInput(midiInputs[index].identifier);
    };
    
    for(int i = 0; i < midiInputs.size(); ++i)
    {
        mMidiInputList.addItem(midiInputs[i].name, i+1);
        if(mDeviceManager.isMidiInputDeviceEnabled(midiInputs[i].identifier))
        {
            setMidiInput(midiInputs[i].identifier);
            mMidiInputList.setSelectedId(i+1);
        }
    }
    
    for(int i = 1; i <= 16; ++i)
    {
        mMidiInputChannelList.addItem(juce::String(i), i);
        mMidiOutputChannelList.addItem(juce::String(i), i);
    }
    
    mMidiInputChannelList.onChange = [this] {
        mMidiInputChannel = mMidiInputChannelList.getSelectedId();
    };
    
    mMidiOutputChannelList.onChange = [this] {
        mMidiOutputChannel = mMidiOutputChannelList.getSelectedId();
    };
    
    mMidiInputList.setTextWhenNoChoicesAvailable("No Midi Outputs Enabled");
    auto midiOutputs = MidiOutput::getAvailableDevices();
    mMidiOutputList.onChange = [this] {
        auto const index = mMidiOutputList.getSelectedItemIndex();
        auto const midiOutputs = MidiOutput::getAvailableDevices();
        setMidiOutput(midiOutputs[index].identifier);
    };
    
    for(int i = 0; i < midiOutputs.size(); ++i)
    {
        mMidiOutputList.addItem(midiOutputs[i].name, i+1);
        
        if(mDeviceManager.getDefaultMidiOutputIdentifier() == midiOutputs[i].identifier)
        {
            setMidiOutput(midiOutputs[i].identifier);
            mMidiOutputList.setSelectedId(i+1);
        }
    }
    
    mWorld.setRect({0, 0, Physics::Utils::pixelsToMeters(static_cast<float>(getWidth())), Physics::Utils::pixelsToMeters(static_cast<float>(getHeight()))});
}

PulsarAudioProcessorEditor::~PulsarAudioProcessorEditor()
{
    stopTimer();
    removeKeyListener(this);
}

//==============================================================================
void PulsarAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    
    Box2DRenderer box2DRenderer;
    box2DRenderer.render(g, mWorld.getWorld(), mWorld.getRect().getX(), mWorld.getRect().getY(), mWorld.getRect().getRight(), mWorld.getRect().getBottom(), getLocalBounds().toFloat());
}

void PulsarAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto ioBounds = bounds.reduced(20, 20).removeFromTop(40);
    auto midiInputBounds = ioBounds.removeFromLeft(ioBounds.getWidth() * 0.5);
    auto midiOutputBounds = ioBounds;
    
    mMidiInputLabel.setBounds(midiInputBounds.removeFromTop(midiInputBounds.getHeight() * 0.5));
    mMidiInputList.setBounds(midiInputBounds.removeFromLeft(midiInputBounds.getWidth() * 0.67));
    mMidiInputChannelList.setBounds(midiInputBounds);
    
    mMidiOutputLabel.setBounds(midiOutputBounds.removeFromTop(midiOutputBounds.getHeight() * 0.5));
    mMidiOutputList.setBounds(midiOutputBounds.removeFromLeft(midiOutputBounds.getWidth() * 0.67));
    mMidiOutputChannelList.setBounds(midiOutputBounds);
}

bool PulsarAudioProcessorEditor::keyPressed(const KeyPress &key, Component *originatingComponent)
{
    if(key == 82) // r
    {
        mWorld.incrementPolygonRotationSpeed();
    }
    else if (key == 73) // i
    {
        mWorld.increaseEdgeSeparation();
    }
    else if (key == 68) // d
    {
        mWorld.decreaseEdgeSeparation();
    }
    else if(key == KeyPress::numberPad3 || key == 51)
    {
        mWorld.createPolygon(3);
    }
    else if(key == KeyPress::numberPad4 || key == 52)
    {
        mWorld.createPolygon(4);
    }
    else if(key == KeyPress::numberPad5 || key == 53)
    {
        mWorld.createPolygon(5);
    }
    else if(key == KeyPress::numberPad6 || key == 54)
    {
        mWorld.createPolygon(6);
    }
    else if(key == KeyPress::numberPad7 || key == 55)
    {
        mWorld.createPolygon(7);
    }
    else if(key == KeyPress::numberPad8 || key == 56)
    {
        mWorld.createPolygon(8);
    }
    else if(key == KeyPress::numberPad9 || key == 57)
    {
        mWorld.createPolygon(9);
    }
    
    return true;
}

void PulsarAudioProcessorEditor::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message)
{
    const ScopedLock s1 (mMidiMonitorLock);
    mIncomingMessages.add(message);
    triggerAsyncUpdate();
}

void PulsarAudioProcessorEditor::handleAsyncUpdate()
{
    // midi message loop
    Array<MidiMessage> messages;
    
    {
        const ScopedLock s1(mMidiMonitorLock);
        messages.swapWith(mIncomingMessages);
    }
    
    for(auto &m : messages)
    {
        if(m.isNoteOn() && m.getChannel() == mMidiInputChannel)
        {
            auto* b = mWorld.spawnBall();
            b->setMidiData(m.getNoteNumber(), m.getVelocity());
        }
    }
}

void PulsarAudioProcessorEditor::sendNoteOnMessage(int noteNumber, float velocity)
{
    //! @todo: Use a midi buffer and do the timestamping properly...?
    auto messageOn = MidiMessage::noteOn(mMidiOutputChannel, noteNumber, static_cast<uint8>(velocity));
    messageOn.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
    
    auto messageOff = MidiMessage::noteOff (messageOn.getChannel(), messageOn.getNoteNumber());
    messageOff.setTimeStamp (messageOn.getTimeStamp() + 0.1);
    
    if(mMidiOutput)
    {
        mMidiOutput->sendMessageNow(messageOn);
        mMidiOutput->sendMessageNow(messageOff);
    }
}

//==============================================================================
void PulsarAudioProcessorEditor::setMidiInput(const String& identifier)
{
    auto list = MidiInput::getAvailableDevices();
    mDeviceManager.removeMidiInputDeviceCallback(identifier, this);
    
    if(!mDeviceManager.isMidiInputDeviceEnabled(identifier))
    {
        mDeviceManager.setMidiInputDeviceEnabled(identifier, true);
    }
    
    mDeviceManager.addMidiInputDeviceCallback(identifier, this);
}

void PulsarAudioProcessorEditor::setMidiOutput(const String& identifier)
{
    auto list = MidiOutput::getAvailableDevices();
    mMidiOutput = MidiOutput::openDevice(identifier);
}
