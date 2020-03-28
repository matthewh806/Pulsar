/*
  ==============================================================================

    Ball.h
    Created: 25 Mar 2020 12:09:58am
    Author:  Matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace Physics
{
    class Ball
    {
    public:
        struct MidiData
        {
            int noteNumber;
            float velocity;
        };
        
        Ball(b2World& world, b2Vec2 pos, double radius = 1.0, float density = 1.0, float restitution = 0.75);
        ~Ball();
        
        void startContact();
        void endContact();
        
        MidiData getMidiData() const;
        
        void setMidiData(int noteNumber, float velocity);
        
        bool isContacting();

        b2Body* getBody();
        
    private:
        bool mContacting = false;
        b2Body* mBody;
        float mRadius;
        
        MidiData mMidiData {1, 0.0};
    };
}

