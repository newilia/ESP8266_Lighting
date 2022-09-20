#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include <Utils.h>

class PlainLightEffect : public ColoredEffect
{
public:
	PlainLightEffect() = default;
	
	void Update() override
	{		
		EVERY_N_MILLIS(10)
		{
			FastLED.showColor(m_color);
		}
	}
};
