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
    
    b2PolygonShape polygonShape;
    
    b2BodyDef polygonBodyDef;
    polygonBodyDef.type = b2_staticBody;
    polygonBodyDef.position.Set(getWidth() * 0.5f, getHeight() * 0.5f - 40.0);
    b2Body* polygonBody = mWorld.CreateBody(&polygonBodyDef);
    
    b2FixtureDef polygonFixtureDef;
    polygonFixtureDef.shape = &polygonShape;
    polygonShape.SetAsBox(40.0, 1.0, b2Vec2(0.0, 0.0), 0.0);
    polygonBody->CreateFixture(&polygonFixtureDef);
    polygonShape.SetAsBox(40.0, 1.0, b2Vec2(0.0, 80.0), 0.0);
    polygonBody->CreateFixture(&polygonFixtureDef);
    polygonShape.SetAsBox(1.0, 40.0, b2Vec2(-40.0, 40.), 0.0);
    polygonBody->CreateFixture(&polygonFixtureDef);
    polygonShape.SetAsBox(1.0, 40.0, b2Vec2(40.0, 40.0), 0.0);
    polygonBody->CreateFixture(&polygonFixtureDef);
    
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
