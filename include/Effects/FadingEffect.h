#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include <SaveData.h>
#include "GlobalDefs.h"

class FadingEffect : public LedsContainer, public ColoredEffect, public SpeedEffect
{
public:
	FadingEffect() = default;

	void Update() override
	{
		EVERY_N_MILLIS(20)
		{
			m_timeArg += 0.1 + m_speed * 0.5;

			int brightness = (int)m_timeArg % 512;
			if (brightness > 255)
			{
				brightness = 511 - brightness;
			}
			brightness = PowNorm(brightness, 255, 2);
			auto color = m_color;
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