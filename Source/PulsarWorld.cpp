/*
  ==============================================================================

    PulsarWorld.cpp
    Created: 25 Mar 2020 12:09:45am
    Author:  Matthew

  ==============================================================================
*/

#include "PulsarWorld.h"
#include "PluginEditor.h"
#include "Utils.h"

Physics::PulsarWorld::PulsarWorld(AudioProcessorEditor& parent, juce::Rectangle<float> worldRect, const b2Vec2& gravity)
: mParent(parent), mWorld(gravity), mWorldRect(worldRect)
{
    startTimer(60);
    
    b2Vec2 polygonPos = {mWorldRect.getWidth() * 0.5f, mWorldRect.getHeight() * 0.5f};
    mPolygon = std::make_unique<Polygon>(mWorld, polygonPos, 6, static_cast<float>(Utils::pixelsToMeters(120.0)));
    
    mWorld.SetContactListener(this);
}

Rectangle<float> const Physics::PulsarWorld::getRect()
{
    return mWorldRect;
}

float const Physics::PulsarWorld::getWidth()
{
    return mWorldRect.getWidth();
}

float const Physics::PulsarWorld::getHeight()
{
    return mWorldRect.getHeight();
}

void Physics::PulsarWorld::setRect(Rectangle<float> rect)
{
    mWorldRect = rect;
}

Physics::Ball* Physics::PulsarWorld::spawnBall(b2Vec2 pos, float radius)
{
    Ball* b = new Ball{mWorld, pos, radius};
    mBalls.push_back(b);
    
    return b;
}

Physics::Ball* Physics::PulsarWorld::spawnBall()
{
    auto const x = mWorldRect.getWidth() * mRandom.nextFloat();
    auto const y = mWorldRect.getHeight() * mRandom.nextFloat();
    
    return spawnBall({x, y}, Utils::pixelsToMeters(mRandom.nextFloat() * 5));
}

void Physics::PulsarWorld::BeginContact(b2Contact* contact)
{
    void* userAData = contact->GetFixtureA()->GetBody()->GetUserData();
    if(userAData != nullptr)
    {
        auto* ball = static_cast<Ball*>(userAData);
        auto const midiData = ball->getMidiData();
        
        auto pulsarAudioEditor = dynamic_cast<PulsarAudioProcessorEditor*>(&mParent);
        pulsarAudioEditor->sendNoteOnMessage(midiData.noteNumber, midiData.velocity);
    }
    
    void* userBData = contact->GetFixtureB()->GetBody()->GetUserData();
    if(userBData != nullptr)
    {
        auto* ball = static_cast<Ball*>(userBData);
        auto const midiData = ball->getMidiData();
        
        auto pulsarAudioEditor = dynamic_cast<PulsarAudioProcessorEditor*>(&mParent);
        pulsarAudioEditor->sendNoteOnMessage(midiData.noteNumber, midiData.velocity);
    }
}

void Physics::PulsarWorld::EndContact(b2Contact* contact)
{

}
