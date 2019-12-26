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
    circleShape.m_radius = 2.0;
    
    b2BodyDef circleBodyDef;
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position.Set(0.0f, 2.0f);
    b2Body* circleBody = mWorld.CreateBody(&circleBodyDef);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    circleBody->CreateFixture(&fixtureDef);
    
    auto const radius = 120.0f;
    createPolygon(getWidth() * 0.5f, getHeight() * 0.5f, 3, radius);
    
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
b2Body* PulsarAudioProcessorEditor::createPolygon(float32 x, float32 y, int32 nSides, float32 radius)
{
    b2PolygonShape polygonShape;
    
    b2BodyDef polygonBodyDef;
    polygonBodyDef.type = b2_staticBody;
    polygonBodyDef.position.Set(x, y);
    b2Body* polygonBody = mWorld.CreateBody(&polygonBodyDef);
    
    b2FixtureDef polygonFixtureDef;
    polygonFixtureDef.shape = &polygonShape;
    
    jassert (nSides > 1);
    
    Point<float32> center { 0, 0 };
    auto angleBetweenPoints = MathConstants<float>::twoPi / static_cast<float>(nSides);
    float startAngle = 0.0;
    
    for(int i = 0; i < nSides; ++i)
    {
        auto const angle = startAngle + i * angleBetweenPoints;
        auto const nextAngle = angle + angleBetweenPoints;
        
        juce::Point<float> const p = { center.x + radius * std::sin(angle), center.y + radius * std::cos(angle) };
        juce::Point<float> const p_next = { center.x + radius * std::sin(nextAngle), center.y + radius * std::cos(nextAngle) };
        auto const boxCenter = b2Vec2((p.getX() + p_next.getX()) * 0.5f, (p.getY() + p_next.getY()) * 0.5f);
        
        polygonShape.SetAsBox(radius, 1.0f, boxCenter, p.getAngleToPoint(p_next) - MathConstants<float>::halfPi);
        polygonBody->CreateFixture(&polygonFixtureDef);
    }
    
    return polygonBody;
}
