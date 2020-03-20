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

class Ball
{
public:
    Ball(b2World& world, b2Vec2 pos, double radius);
    ~Ball() = default;
    
private:
    b2CircleShape mCircleShape;
    b2BodyDef mCircleBodyDef;
    b2FixtureDef mFixtureDef;
};

class PulsarWorld : public b2World
{
public:
    PulsarWorld(juce::Rectangle<float> worldRect, const b2Vec2& gravity);
    
    Rectangle<float> const getRect();
    float const getWidth();
    float const getHeight();
    
private:
    Rectangle<float> mWorldRect;
};

class Polygon
{
public:
    Polygon(b2World& world, b2Vec2 pos, int32 nSides, double radius, float startAngle = 0.0, b2Vec2 center = {0.0, 0.0});
    
    b2Body* getBody();
    
private:
    b2Body* mPolygonBody;
};

class PulsarAudioProcessorEditor  : public AudioProcessorEditor, Timer, b2ContactListener
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
    
    //b2ContactListener
    /// Called when two fixtures begin to touch.
    void BeginContact(b2Contact* contact) override;

    /// Called when two fixtures cease to touch.
    void EndContact(b2Contact* contact) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PulsarAudioProcessor& processor;
    
    DrawablePath mShape;
    
    PulsarWorld mWorld {{ 0.0f, 0.0f, 4.0f, 4.0f }, {0.0f, 10.0f}};
    
    std::vector<Ball> mBalls;
    Polygon mPolygon;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsarAudioProcessorEditor)
};
