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
    
    addAndMakeVisible(mMidiInputList);
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
            mMidiInputList.setSelectedId(i);
        }
    }
    
    mWorld.setRect({0, 0, Utils::pixelsToMeters(static_cast<float>(getWidth())), Utils::pixelsToMeters(static_cast<float>(getHeight()))});
}

PulsarAudioProcessorEditor::~PulsarAudioProcessorEditor()
{
    stopTimer();
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
    auto const bounds = getLocalBounds();
    mMidiInputList.setBounds(bounds.getX() + 20, bounds.getY()+20, 100, 20);
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
    
//    String messageText;
    for(auto &m : messages)
    {
        std::cout << "Message: " + m.getDescription() << "\n";
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
