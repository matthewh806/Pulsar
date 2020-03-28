/*
  ==============================================================================

    Ball.cpp
    Created: 25 Mar 2020 12:09:58am
    Author:  Matthew

  ==============================================================================
*/

#include "Ball.h"

Physics::Ball::Ball(b2World& world, b2Vec2 pos, double radius, float density, float restitution)
{
    
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = radius;
    
    b2BodyDef circleBodyDef;
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position.Set(pos.x, pos.y);
    mBody = world.CreateBody(&circleBodyDef);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = density;
    fixtureDef.restitution = restitution;
    fixtureDef.friction = 0.0f;
    mBody->CreateFixture(&fixtureDef);
    
    mBody->SetUserData(this);
}

Physics::Ball::~Ball()
{
    mBody->GetWorld()->DestroyBody( mBody );
}

void Physics::Ball::startContact()
{
    mContacting = true;
}

void Physics::Ball::endContact()
{
    mContacting = false;
}

bool Physics::Ball::isContacting()
{
    return mContacting;
}

b2Body* Physics::Ball::getBody()
{
    return mBody;
}

Physics::Ball::MidiData Physics::Ball::getMidiData() const
{
    return mMidiData;
}

void Physics::Ball::setMidiData(int noteNumber, float velocity)
{
    mMidiData.noteNumber = noteNumber;
    mMidiData.velocity = velocity;
}
