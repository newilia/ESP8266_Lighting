#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include <Utils.h>

class PlainLightEffect : public IEffect, public SingleColoredEffect
{
public:
	PlainLightEffect(const CRGB * color)
	: SingleColoredEffect(color) {}

	PlainLightEffect(CRGB color)
	: SingleColoredEffect(color) {}

	void Update() override
	{		
		EVERY_N_MILLIS(10)
		{
			FastLED.showColor(*m_color);
		}
	}
};
