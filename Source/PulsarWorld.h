/*
  ==============================================================================

    PulsarWorld.h
    Created: 25 Mar 2020 12:09:45am
    Author:  Matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Ball.h"
#include "Polygon.h"

namespace Physics
{
    class PulsarWorld : public Timer, b2ContactListener
    {
    public:
        PulsarWorld(AudioProcessorEditor& parent, juce::Rectangle<float> worldRect, const b2Vec2& gravity);
        
        Rectangle<float> const getRect();
        float const getWidth();
        float const getHeight();
        
        b2World& getWorld()
        {
            return mWorld;
        }
        
        void setRect(Rectangle<float> rect);
        
        void createPolygon(int nSides);
        
        void incrementPolygonRotationSpeed();
        void increaseEdgeSeparation();
        void decreaseEdgeSeparation();
        
        //b2ContactListener
        /// Called when two fixtures begin to touch.
        void BeginContact(b2Contact* contact) override;

        /// Called when two fixtures cease to touch.
        void EndContact(b2Contact* contact) override;
        
        Physics::Ball* spawnBall();
        Physics::Ball* spawnBall(b2Vec2 pos, float radius);
        
        void timerCallback() override
        {
            mWorld.Step(0.02, 8, 3);
            mParent.repaint();
        }
        
    private:
        AudioProcessorEditor& mParent;
        
        b2World mWorld;
        Rectangle<float> mWorldRect;
        
        std::vector<Ball*> mBalls;
        std::unique_ptr<Polygon> mPolygon;
        
        b2Body* mWorldBoundary;
        
        Random mRandom;
    };
}

