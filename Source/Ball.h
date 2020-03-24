/*
  ==============================================================================

    Ball.h
    Created: 25 Mar 2020 12:09:58am
    Author:  Matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Ball
{
public:
    Ball(b2World& world, b2Vec2 pos, double radius = 1.0, float density = 1.0, float restitution = 0.75);
    ~Ball() = default;
    
    void startContact();
    void endContact();
    
    bool isContacting();

    b2Body* getBody();
    
private:
    bool mContacting = false;
    b2Body* mBody;
    float mRadius;
};

