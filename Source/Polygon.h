/*
  ==============================================================================

    Polygon.h
    Created: 25 Mar 2020 12:10:11am
    Author:  Matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace Physics
{
    class Polygon
    {
    public:
        Polygon(b2World& world, b2Vec2 pos, int32 nSides, double radius, float startAngle = 0.0, b2Vec2 center = {0.0, 0.0});
        
        b2Body* getBody();
        
        float getRadius();
        bool testPoint(b2Vec2 const &p);
        
        b2Vec2 getRandomPointInside();
        
        void incrementRotation();
        void increaseEdgeSeparation(int amount);
        
        float getAngularVelocity();
        void setAngularVelocity(float angularVelocity);
        
    private:
        b2Body* mPolygonBody;
        float mRadius;
        int32 mNumSides;
        b2PolygonShape mPolygonShape;
        
        b2PolygonShape mPolygonVertexShape;
        b2Vec2 mVertices[9];
    };
}
