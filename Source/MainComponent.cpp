/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "MainComponent.h"
#include "Utils.h"

//==============================================================================

MainComponent::MainComponent ()
{
    setSize (400, 400);
    
    addKeyListener(this);
    
    mDeviceManager.initialiseWithDefaultDevices(0, 0);
    mDeviceManager.addChangeListener(this);
    
    addAndMakeVisible(mMidiChannelLabel);
    addAndMakeVisible(mMidiInputChannelList);
    addAndMakeVisible(mMidiOutputChannelList);
    
    mGravitySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mGravitySlider.setRange(juce::Range<double>{-20.0, 20.0}, 1);
    mGravitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    addAndMakeVisible(mGravitySlider);
    
    mRotationSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mRotationSlider.setRange(juce::Range<double>{-360, 360}, 1);
    mRotationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    addAndMakeVisible(mRotationSlider);
    
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
    
    mWorld.setRect({0, 0, Physics::Utils::pixelsToMeters(static_cast<float>(getWidth())), Physics::Utils::pixelsToMeters(static_cast<float>(getHeight()))});
}

MainComponent::~MainComponent()
{
    removeKeyListener(this);
    
    mDeviceManager.removeChangeListener(this);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    
    g.drawSingleLineText("Number of balls: " + juce::String(mWorld.getNumberOfBalls()), 20, getHeight() - 40);
    
    Box2DRenderer box2DRenderer;
    box2DRenderer.render(g, mWorld.getWorld(), mWorld.getRect().getX(), mWorld.getRect().getY(), mWorld.getRect().getRight(), mWorld.getRect().getBottom(), getLocalBounds().toFloat());
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    auto ioBounds = bounds.removeFromTop(40);
    auto ctrlBounds = bounds.removeFromBottom(100);
    auto midiInputBounds = ioBounds.removeFromLeft(ioBounds.getWidth() * 0.5);
    auto midiOutputBounds = ioBounds;
    
    mMidiChannelLabel.setBounds(midiInputBounds.removeFromTop(midiInputBounds.getHeight() * 0.5));
    mMidiInputChannelList.setBounds(midiInputBounds);
    mMidiOutputChannelList.setBounds(midiOutputBounds);
    
    mGravitySlider.setBounds(ctrlBounds.removeFromLeft(100));
    mRotationSlider.setBounds(ctrlBounds.removeFromLeft(100));
}

bool MainComponent::keyPressed(const KeyPress &key, Component *originatingComponent)
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

void MainComponent::mouseUp (const MouseEvent& event)
{
    b2Vec2 const worldPos {Physics::Utils::pixelsToMeters(event.position.x), Physics::Utils::pixelsToMeters(event.position.y)};
    if(mWorld.testPointInPolygon(worldPos))
    {
        mWorld.spawnBall(worldPos);
    }
}

void MainComponent::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message)
{
    const ScopedLock s1 (mMidiMonitorLock);
    mIncomingMessages.add(message);
    triggerAsyncUpdate();
}

void MainComponent::handleAsyncUpdate()
{
    // midi message loop
    Array<MidiMessage> messages;
    
    {
        const ScopedLock s1(mMidiMonitorLock);
        messages.   swapWith(mIncomingMessages);
    }
    
    for(auto &m : messages)
    {
        if(m.getChannel() != mMidiInputChannel)
        {
            continue;
        }
        
        if(m.isNoteOn())
        {
            mWorld.spawnBall(m.getNoteNumber(), m.getVelocity());
        }
        else if(m.isController())
        {
            auto const pitchVal = m.getControllerValue();
            
            // map 0 - 127 to 0 - 360
            auto anglularVelocity = 360.0 / static_cast<double>(127) * static_cast<double>(pitchVal);
            mWorld.setPolygonRotationSpeed(anglularVelocity);
        }
    }
}

void MainComponent::sendNoteOnMessage(int noteNumber, float velocity)
{
    if(!mMidiOutput)
    {
        return;
    }
    
    //! @todo: Use a midi buffer and do the timestamping properly...?
    auto messageOn = MidiMessage::noteOn(mMidiOutputChannel, noteNumber, static_cast<uint8>(velocity));
    messageOn.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
    
    auto messageOff = MidiMessage::noteOff (messageOn.getChannel(), messageOn.getNoteNumber());
    messageOff.setTimeStamp (messageOn.getTimeStamp() + 0.1);
    mMidiOutput->sendMessageNow(messageOn);
    mMidiOutput->sendMessageNow(messageOff);
}

//==============================================================================
void MainComponent::changeListenerCallback (juce::ChangeBroadcaster* source)
{
//    getDeviceManager();
    std::cout << mDeviceManager.getDefaultMidiOutput() << std::endl;
}
