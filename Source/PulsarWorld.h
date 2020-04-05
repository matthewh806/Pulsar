/*
  ==============================================================================

    PulsarWorld.h
    Created: 25 Mar 2020 12:09:45am
    Author:  Matthew

  ==============================================================================
*/

#pragma once

#include <set>
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
        
        size_t getNumberOfBalls();
        
        void createPolygon(int nSides);
        
        //! @brief: Set the angular velocity of the polyon.
        //! value will wrap around range  [0, 360]
        void setPolygonRotationSpeed(double angularVelocity);
        void incrementPolygonRotationSpeed();
        
        void increaseEdgeSeparation();
        void decreaseEdgeSeparation();
        
        bool testPointInPolygon(b2Vec2 const &p);
        
        //b2ContactListener
        /// Called when two fixtures begin to touch.
        void BeginContact(b2Contact* contact) override;

        /// Called when two fixtures cease to touch.
        void EndContact(b2Contact* contact) override;
        
        Physics::Ball* spawnBall(int noteNumber, float velocity);
        
        //! @brief: Spawn a ball at pos and with random radius
        //!
        //! Note: Coordinates should be scaled to physics coordinates
        Physics::Ball* spawnBall(b2Vec2 pos);
        
        //! @brief: Spawn a ball at pos and with radius
        //!
        //! Note: Coordinates should be scaled to physics coordinates
        Physics::Ball* spawnBall(b2Vec2 pos, float radius, int noteNumber, float velocity);
        
        //! @brief: remove all balls scheduled to be removed from the scene.
        //! should be called outside step method!
        //! also clears the list of balls to be removed.
        void removeBalls();
        
        void timerCallback() override
        {
            removeBalls();
            
            mWorld.Step(0.02, 8, 3);
            mParent.repaint();
        }
        
    private:
        void removeBall(Ball* ball);
        
        AudioProcessorEditor& mParent;
        
        b2World mWorld;
        Rectangle<float> mWorldRect;
        
        std::vector<Ball*> mBalls;
        std::set<Ball*> mBallsToRemove;

        std::unique_ptr<Polygon> mPolygon;
        
        b2Body* mWorldBoundary;
        
        Random mRandom;
    };
}

