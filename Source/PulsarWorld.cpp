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

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

Physics::PulsarWorld::PulsarWorld(AudioProcessorEditor& parent, juce::Rectangle<float> worldRect, const b2Vec2& gravity)
: mParent(parent), mWorld(gravity), mWorldRect(worldRect)
{
    startTimer(60);
    
    b2BodyDef boundaryBodyDef;
    boundaryBodyDef.type = b2_staticBody;
    boundaryBodyDef.position.Set(0, 0);
    mWorldBoundary = mWorld.CreateBody(&boundaryBodyDef);
    
    b2PolygonShape boundaryPolygonShape;
    
    b2FixtureDef borderFixtureDef;
    borderFixtureDef.shape = &boundaryPolygonShape;
    borderFixtureDef.isSensor = true;
    
    boundaryPolygonShape.SetAsBox(getRect().getWidth() * 0.5, Utils::pixelsToMeters(1), {static_cast<float32>((getRect().getWidth() * 0.5)), static_cast<float32>(getRect().getHeight())}, 0); // bottom
    mWorldBoundary->CreateFixture(&borderFixtureDef);
    boundaryPolygonShape.SetAsBox(getRect().getWidth() * 0.5, Utils::pixelsToMeters(1), {static_cast<float32>((getRect().getWidth() * 0.5)), static_cast<float32>(getRect().getY())}, 0); // top
    mWorldBoundary->CreateFixture(&borderFixtureDef);
    boundaryPolygonShape.SetAsBox(Utils::pixelsToMeters(1), getRect().getHeight() * 0.5, {static_cast<float32>((getRect().getX())), static_cast<float32>(getRect().getHeight() * 0.5)}, 0); // left
    mWorldBoundary->CreateFixture(&borderFixtureDef);
    boundaryPolygonShape.SetAsBox(Utils::pixelsToMeters(1), getRect().getHeight() * 0.5, {static_cast<float32>((getRect().getRight())), static_cast<float32>(getRect().getHeight() * 0.5)}, 0); // right
    mWorldBoundary->CreateFixture(&borderFixtureDef);
    
    createPolygon(3);
    
    mWorld.SetContactListener(this);
}

Rectangle<float> const Physics::PulsarWorld::getRect()
{
    return mWorldRect;
}

size_t Physics::PulsarWorld::getNumberOfBalls()
{
    return mBalls.size();
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

void Physics::PulsarWorld::createPolygon(int nSides)
{
    if(mPolygon != nullptr)
    {
        for(auto* b : mBalls)
        {
            mWorld.DestroyBody(b->getBody());
        }
        mBalls.clear();
        
        mWorld.DestroyBody(mPolygon->getBody());
        mPolygon.release();
    }
    
    mBalls.clear();
    
    b2Vec2 polygonPos = {mWorldRect.getWidth() * 0.5f, mWorldRect.getHeight() * 0.5f};
    mPolygon = std::make_unique<Polygon>(mWorld, polygonPos, nSides, static_cast<float>(Utils::pixelsToMeters(120.0)));
}

void Physics::PulsarWorld::setPolygonRotationSpeed(double speed)
{
    mPolygon->setAngularVelocity( std::fmod(speed * DEGTORAD, 360 * DEGTORAD ));
}

void Physics::PulsarWorld::incrementPolygonRotationSpeed()
{
    auto const curAngVelocity = mPolygon->getAngularVelocity();
    mPolygon->setAngularVelocity( std::fmod(curAngVelocity + 45 * DEGTORAD, 360 * DEGTORAD ));
}

void Physics::PulsarWorld::increaseEdgeSeparation()
{
    mPolygon->increaseEdgeSeparation(2);
}

void Physics::PulsarWorld::decreaseEdgeSeparation()
{
    mPolygon->increaseEdgeSeparation(-2);
}

bool Physics::PulsarWorld::testPointInPolygon(b2Vec2 const &p)
{
    return mPolygon->testPoint(p);
}

Physics::Ball* Physics::PulsarWorld::spawnBall(b2Vec2 pos)
{
    //! @todo: add an option to keep in a particular key
    int const midiNote = mRandom.nextInt(127);
    int const velocity = mRandom.nextInt(127);
    return spawnBall(pos, Utils::pixelsToMeters(mRandom.nextFloat() * 5), midiNote, velocity);
}

Physics::Ball* Physics::PulsarWorld::spawnBall(b2Vec2 pos, float radius, int noteNumber, float velocity)
{
    Ball* b = new Ball{mWorld, pos, radius};
    b->setMidiData(noteNumber, velocity);
    mBalls.push_back(b);
    
    return b;
}

Physics::Ball* Physics::PulsarWorld::spawnBall(int noteNumber, float velocity)
{
    return spawnBall(mPolygon->getRandomPointInside(), Utils::pixelsToMeters(mRandom.nextFloat() * 5), noteNumber, velocity);
}

void Physics::PulsarWorld::removeBalls()
{
    for(auto it = mBallsToRemove.begin(); it != mBallsToRemove.end(); ++it)
    {
        removeBall(*it);
    }
    
    mBallsToRemove.clear();
}

void Physics::PulsarWorld::BeginContact(b2Contact* contact)
{
    void* userAData = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userBData = contact->GetFixtureB()->GetBody()->GetUserData();
    
    if(!contact->GetFixtureA()->IsSensor() && !contact->GetFixtureB()->IsSensor())
    {
        if(userAData)
        {
            auto* ball = static_cast<Ball*>(userAData);
            auto const midiData = ball->getMidiData();
        
            auto pulsarAudioEditor = dynamic_cast<PulsarAudioProcessorEditor*>(&mParent);
            pulsarAudioEditor->sendNoteOnMessage(midiData.noteNumber, midiData.velocity);
        }
        
        if(userBData)
        {
            auto* ball = static_cast<Ball*>(userBData);
            auto const midiData = ball->getMidiData();
        
            auto pulsarAudioEditor = dynamic_cast<PulsarAudioProcessorEditor*>(&mParent);
            pulsarAudioEditor->sendNoteOnMessage(midiData.noteNumber, midiData.velocity);
        }
        
        return;
    }
    
    if(contact->GetFixtureA()->IsSensor() && !contact->GetFixtureB()->IsSensor())
    {
        auto* ball = static_cast<Ball*>(userBData);
        mBallsToRemove.insert(ball);
        
        return;
    }
    
    if(contact->GetFixtureB()->IsSensor() && !contact->GetFixtureA()->IsSensor())
    {
        auto* ball = static_cast<Ball*>(userAData);
        mBallsToRemove.insert(ball);
        
        return;
    }
}

void Physics::PulsarWorld::EndContact(b2Contact* contact)
{

}

void Physics::PulsarWorld::removeBall(Ball* ball)
{
    for(auto it = mBalls.begin(); it != mBalls.end();)
    {
        if(*it == ball)
        {
            delete ball;
            mBalls.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
