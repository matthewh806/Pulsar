/*
  ==============================================================================

    PulsarWorld.cpp
    Created: 25 Mar 2020 12:09:45am
    Author:  Matthew

  ==============================================================================
*/

#include "PulsarWorld.h"
#include "Utils.h"

Physics::PulsarWorld::PulsarWorld(Component& parent, juce::Rectangle<float> worldRect, const b2Vec2& gravity)
: mParent(parent), mWorld(gravity), mWorldRect(worldRect)
{
    startTimer(60);
    
    b2Vec2 polygonPos = {mWorldRect.getWidth() * 0.5f, mWorldRect.getHeight() * 0.5f};
    mPolygon = std::make_unique<Polygon>(mWorld, polygonPos, 6, static_cast<float>(Utils::pixelsToMeters(120.0)));
    
    mWorld.SetContactListener(this);

    mBalls.push_back(Ball{mWorld, {mWorldRect.getWidth() * 0.3f, mWorldRect.getHeight() * 0.5f}, Utils::pixelsToMeters(2.0)});
    mBalls.push_back(Ball{mWorld, {mWorldRect.getWidth() * 0.5f, mWorldRect.getHeight() * 0.5f}, Utils::pixelsToMeters(8.0)});
    mBalls.push_back(Ball{mWorld, {mWorldRect.getWidth() * 0.7f, mWorldRect.getHeight() * 0.5f}, Utils::pixelsToMeters(4.0)});
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

void Physics::PulsarWorld::BeginContact(b2Contact* contact)
{
//    std::cout << "Contact begin" << std::endl;
}

void Physics::PulsarWorld::EndContact(b2Contact* contact)
{
//    std::cout << "Contact end" << std::endl;
}
