/*
  ==============================================================================

    Polygon.cpp
    Created: 25 Mar 2020 12:10:11am
    Author:  Matthew

  ==============================================================================
*/

#include "Polygon.h"
#include "Utils.h"

Physics::Polygon::Polygon(b2World& world, b2Vec2 pos, int32 nSides, double radius, float startAngle, b2Vec2 center)
{
    mNumSides = nSides;
    mRadius = radius;
    
    for(int i = 0; i < 9; ++i)
    {
        mVertices[i] = {0.0, 0.0};
    }
    
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
    
    for(int i = 0; i < nSides; ++i)
    {
        auto const angle = startAngle + i * angleBetweenPoints;
        auto const nextAngle = angle + angleBetweenPoints;
        
        b2Vec2 const p(center.x + radius * std::sin(-angle), center.y + radius * std::cos(-angle));
        b2Vec2 const p_next(center.x + radius * std::sin(-nextAngle), center.y + radius * std::cos(-nextAngle));
        b2Vec2 const boxCenter((p.x + p_next.x) * 0.5f, (p.y + p_next.y) * 0.5f);
        auto const boxAngle = std::atan2(p_next.y - p.y, p_next.x - p.x);
        
        mVertices[i].Set(center.x + radius * std::sin(-angle), center.y + radius * std::cos(-angle));
        polygonShape.SetAsBox(sideLength * 0.5f, Utils::pixelsToMeters(1.0f), boxCenter, boxAngle);
        mPolygonBody->CreateFixture(&polygonFixtureDef);
    }
    
    mPolygonVertexShape.Set(mVertices, nSides);
}

b2Body* Physics::Polygon::getBody()
{
    return mPolygonBody;
}

bool Physics::Polygon::testPoint(b2Vec2 const &p)
{
    return mPolygonVertexShape.TestPoint(mPolygonBody->GetTransform(), p);
}

b2Vec2 Physics::Polygon::getRandomPointInside()
{
    b2Vec2 pt;
    auto const pos = mPolygonBody->GetPosition();
    
    while(true)
    {
        pt = { Utils::RandomFloat(pos.x - mRadius, pos.x + mRadius), Utils::RandomFloat(pos.y - mRadius, pos.y + mRadius) };
        
        if(testPoint(pt))
        {
            break;
        }
    }

    return pt;
}

void Physics::Polygon::increaseEdgeSeparation(int amount)
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
            verts[i] = {v.x + Utils::pixelsToMeters(amount) * std::cos(angle), v.y + Utils::pixelsToMeters(amount) * std::sin(angle)};
        }
        
        ++idx;
    }
}

float Physics::Polygon::getAngularVelocity()
{
    return mPolygonBody->GetAngularVelocity();
}

void Physics::Polygon::setAngularVelocity(float angularVelocity)
{
    mPolygonBody->SetAngularVelocity(angularVelocity);
}
