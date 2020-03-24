/*
  ==============================================================================

    PulsarWorld.h
    Created: 25 Mar 2020 12:09:45am
    Author:  Matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ball.h"
#include "polygon.h"

class PulsarWorld : public Timer, b2ContactListener
{
public:
    PulsarWorld(Component& parent, juce::Rectangle<float> worldRect, const b2Vec2& gravity);
    
    Rectangle<float> const getRect();
    float const getWidth();
    float const getHeight();
    
    b2World& getWorld()
    {
        return mWorld;
    }
    
    void setRect(Rectangle<float> rect);
    
    //b2ContactListener
    /// Called when two fixtures begin to touch.
    void BeginContact(b2Contact* contact) override;

    /// Called when two fixtures cease to touch.
    void EndContact(b2Contact* contact) override;
    
    void timerCallback() override
    {
        mWorld.Step(0.02, 8, 3);
        mParent.repaint();
    }
    
private:
    Component& mParent;
    
    b2World mWorld;
    Rectangle<float> mWorldRect;
    
    std::vector<Ball> mBalls;
    std::unique_ptr<Polygon> mPolygon;
};
