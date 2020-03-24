/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

static const float pixelsPerMeter = 100;

static float pixelsToMeters(float vPixel)
{
    return vPixel / pixelsPerMeter;
}

static float metersToPixels(float vMeter)
{
    return vMeter * pixelsPerMeter;
}

static b2Vec2 pixelsToMeters(float xPixels, float yPixels)
{
    return b2Vec2( xPixels / pixelsPerMeter, yPixels / pixelsPerMeter);
}

static b2Vec2 metersToPixels(float xMeters, float yMeters)
{
    return b2Vec2(xMeters * pixelsPerMeter, yMeters * pixelsPerMeter);
}

Ball::Ball(b2World& world, b2Vec2 pos, double radius, float density, float restitution)
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

void Ball::startContact()
{
    mContacting = true;
}

void Ball::endContact()
{
    mContacting = false;
}

bool Ball::isContacting()
{
    return mContacting;
}

b2Body* Ball::getBody()
{
    return mBody;
}

Polygon::Polygon(b2World& world, b2Vec2 pos, int32 nSides, double radius, float startAngle, b2Vec2 center)
{
    mNumSides = nSides;
    
    b2PolygonShape polygonShape;
    
    b2BodyDef polygonBodyDef;
    polygonBodyDef.type = b2_kinematicBody;
    polygonBodyDef.position.Set(pos.x, pos.y);
    mPolygonBody = world.CreateBody(&polygonBodyDef);
    
    b2FixtureDef polygonFixtureDef;
    polygonFixtureDef.shape = &polygonShape;
    polygonFixtureDef.restitution = 0.0f;
    
    jassert (nSides > 1);
    
    auto const angleBetweenPoints = MathConstants<float>::twoPi / static_cast<float>(nSides);
    auto const sideLength = 2 * radius * std::sin(MathConstants<float>::pi / static_cast<float>(nSides));
    
    b2Vec2 vertices[nSides];
    for(int i = 0; i < nSides; ++i)
    {
        auto const angle = startAngle + i * angleBetweenPoints;
        auto const nextAngle = angle + angleBetweenPoints;
        
        b2Vec2 const p(center.x + radius * std::sin(angle), center.y + radius * std::cos(angle));
        b2Vec2 const p_next(center.x + radius * std::sin(nextAngle), center.y + radius * std::cos(nextAngle));
        b2Vec2 const boxCenter((p.x + p_next.x) * 0.5f, (p.y + p_next.y) * 0.5f);
        auto const boxAngle = std::atan2(p_next.y - p.y, p_next.x - p.x);
        
        polygonShape.SetAsBox(sideLength * 0.5f, pixelsToMeters(1.0f), boxCenter, boxAngle);
        mPolygonBody->CreateFixture(&polygonFixtureDef);
    }
    
    mPolygonBody->CreateFixture(&polygonFixtureDef);
}

b2Body* Polygon::getBody()
{
    return mPolygonBody;
}

void Polygon::increaseEdgeSeparation(int amount)
{
    int idx = 0;
    for (b2Fixture* f = mPolygonBody->GetFixtureList(); f; f = f->GetNext())
    {
        auto verts = static_cast<b2PolygonShape*>(f->GetShape())->m_vertices;
        auto norms = static_cast<b2PolygonShape*>(f->GetShape())->m_normals;
        auto angle = std::atan2(norms[1].y, norms[1].x);
        
        for(int i = 0; i < 4; i++)
        {
            auto const v = verts[i];
            verts[i] = {v.x + amount * std::cos(angle), v.y + amount * std::sin(angle)};
        }
        
        ++idx;
    }
}

float Polygon::getAngularVelocity()
{
    return mPolygonBody->GetAngularVelocity();
}

void Polygon::setAngularVelocity(float angularVelocity)
{
    mPolygonBody->SetAngularVelocity(angularVelocity);
}

PulsarWorld::PulsarWorld(juce::Rectangle<float> worldRect, const b2Vec2& gravity)
: b2World(gravity), mWorldRect(worldRect)
{
    
}

Rectangle<float> const PulsarWorld::getRect()
{
    return mWorldRect;
}

float const PulsarWorld::getWidth()
{
    return mWorldRect.getWidth();
}

float const PulsarWorld::getHeight()
{
    return mWorldRect.getHeight();
}

void PulsarWorld::setRect(Rectangle<float> rect)
{
    mWorldRect = rect;
}

PulsarAudioProcessorEditor::PulsarAudioProcessorEditor (PulsarAudioProcessor& p)
: AudioProcessorEditor (&p)
, processor (p)
{
    setSize (400, 400);
    mWorld.setRect({0, 0, pixelsToMeters(static_cast<float>(getWidth())), pixelsToMeters(static_cast<float>(getHeight()))});
    
    b2Vec2 polygonPos = {mWorld.getWidth() * 0.5f, mWorld.getHeight() * 0.5f};
    mPolygon = std::make_unique<Polygon>(mWorld, polygonPos, 6, static_cast<float>(pixelsToMeters(120.0)));

    mBalls.push_back(Ball{mWorld, pixelsToMeters(getWidth() * 0.3f, getHeight() * 0.5f), pixelsToMeters(2.0)});
    mBalls.push_back(Ball{mWorld, pixelsToMeters(getWidth() * 0.5f, getHeight() * 0.5f), pixelsToMeters(8.0)});
    mBalls.push_back(Ball{mWorld, pixelsToMeters(getWidth() * 0.7f, getHeight() * 0.5f), pixelsToMeters(4.0)});
    
    mWorld.SetContactListener(this);
    startTimer(60);
}

PulsarAudioProcessorEditor::~PulsarAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void PulsarAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    
    Box2DRenderer box2DRenderer;
    box2DRenderer.render(g, mWorld, mWorld.getRect().getX(), mWorld.getRect().getY(), mWorld.getRect().getRight(), mWorld.getRect().getBottom(), getLocalBounds().toFloat());
}

void PulsarAudioProcessorEditor::resized()
{
}

//==============================================================================
void PulsarAudioProcessorEditor::BeginContact(b2Contact* contact)
{
//    std::cout << "Contact begin" << std::endl;
}

void PulsarAudioProcessorEditor::EndContact(b2Contact* contact)
{
//    std::cout << "Contact end" << std::endl;
}
