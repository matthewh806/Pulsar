/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PulsarAudioProcessorEditor  : public AudioProcessorEditor, Timer
{
public:
    PulsarAudioProcessorEditor (PulsarAudioProcessor&);
    ~PulsarAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override
    {
        mWorld.Step(getTimerInterval(), 8, 3);
        repaint();
    }

private:
    b2Body* createPolygon(float32 x, float32 y, int32 nSides, float32 radius);
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PulsarAudioProcessor& processor;
    
    DrawablePath mShape;
    
    b2Vec2 mGravity { 0.0f, 10.0f };
    b2World mWorld { mGravity };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsarAudioProcessorEditor)
};
