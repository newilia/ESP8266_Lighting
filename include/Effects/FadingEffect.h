#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include "GlobalDefs.h"

class FadingEffect : public LedsContainer, public ColoredEffect, public SpeedEffect
{
public:
	FadingEffect() = default;

	void Update() override
	{
		EVERY_N_MILLIS(EFFECTS_PERIOD_DEFAULT)
		{
			m_timeArg += 1 + m_speed * 5;

			int brightness = (int)m_timeArg % 512;
			if (brightness > 255)
			{
				brightness = 511 - brightness;
			}
			brightness = PowNorm(brightness, 255, 2);
			auto color = GetColor();
			color.nscale8(brightness);

			for (int i = 0; i < m_leds.second; ++i)
			{
				m_leds.first[i] = color;
			}
			FastLED.show();
		}
	}
private:
	float	m_timeArg = 0;
};