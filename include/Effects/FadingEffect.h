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
			UpdateTime(0.2, 1);

			int brightness = (int)m_time % 512;
			if (brightness > 255)
			{
				brightness = 511 - brightness;
			}
			brightness = PowNorm(brightness, 255, 2);
			auto color = GetColor();
			color.nscale8(brightness);

			for (auto * led = m_leds.start; led < m_leds.end; ++led)
			{
				*led = color;
			}
			FastLED.show();
		}
	}
private:
};