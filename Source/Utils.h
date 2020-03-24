/*
  ==============================================================================

    Utils.h
    Created: 25 Mar 2020 12:18:58am
    Author:  Matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace Physics
{
    class Utils
    {
    public:
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
        
        constexpr static const float pixelsPerMeter = 100;
    };
}
