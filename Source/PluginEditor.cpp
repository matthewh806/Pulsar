/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PulsarAudioProcessorEditor::PulsarAudioProcessorEditor (PulsarAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    
    mShape.setFill(Colours::transparentWhite);
    mShape.setStrokeFill(Colours::pink);
    mShape.setStrokeThickness(2.0);
    addAndMakeVisible(mShape);
    
    b2CircleShape circleShape;
    circleShape.m_p.Set(getWidth() * 0.5f, getHeight() * 0.5f);
    circleShape.m_radius = 4.0;
    
    b2BodyDef circleBodyDef;
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position.Set(0.0f, 2.0f);
    b2Body* circleBody = mWorld.CreateBody(&circleBodyDef);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 0.75f;
    fixtureDef.restitution = 1.0f;
    fixtureDef.friction = 0.0f;
    circleBody->CreateFixture(&fixtureDef);
    
    createPolygon(getWidth() * 0.5f, getHeight() * 0.5f, 4, 120.0f, MathConstants<float>::pi / 4.0f);
    
    startTimer(100);
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
    const Rectangle<float> targetArea { static_cast<float>(getX()), static_cast<float>(getY()), static_cast<float>(getWidth()), static_cast<float>(getHeight()) };
    box2DRenderer.render(g, mWorld, getX(), getY(), getRight(), getBottom(), targetArea);
}

void PulsarAudioProcessorEditor::resized()
{
}

//==============================================================================
b2Body* PulsarAudioProcessorEditor::createPolygon(float32 x, float32 y, int32 nSides, float32 radius, float startAngle)
{
    b2PolygonShape polygonShape;
    
    b2BodyDef polygonBodyDef;
    polygonBodyDef.type = b2_staticBody;
    polygonBodyDef.position.Set(x, y);
    b2Body* polygonBody = mWorld.CreateBody(&polygonBodyDef);
    
    b2FixtureDef polygonFixtureDef;
    polygonFixtureDef.shape = &polygonShape;
    polygonFixtureDef.restitution = 0.0f;
    
    jassert (nSides > 1);
    
    Point<float32> center { 0, 0 };
    auto const angleBetweenPoints = MathConstants<float>::twoPi / static_cast<float>(nSides);
    
    auto const sideLength = 2 * radius * std::sin(MathConstants<float>::pi / static_cast<float>(nSides));
    for(int i = 0; i < nSides; ++i)
    {
        auto const angle = startAngle + i * angleBetweenPoints;
        auto const nextAngle = angle + angleBetweenPoints;
        
        juce::Point<float> const p = { center.x + radius * std::sin(angle), center.y + radius * std::cos(angle) };
        juce::Point<float> const p_next = { center.x + radius * std::sin(nextAngle), center.y + radius * std::cos(nextAngle) };
        auto const boxCenter = b2Vec2((p.getX() + p_next.getX()) * 0.5f, (p.getY() + p_next.getY()) * 0.5f);
        auto const boxAngle = std::atan2(p_next.y - p.y, p_next.x - p.x);
        
        polygonShape.SetAsBox(sideLength * 0.5f, 1.0f, boxCenter, boxAngle);
        polygonBody->CreateFixture(&polygonFixtureDef);
    }
    
    return polygonBody;
}
