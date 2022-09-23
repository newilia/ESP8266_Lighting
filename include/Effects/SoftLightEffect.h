#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include <Utils.h>

class SoftLightEffect : public LedsContainer, public ColoredEffect
{
public:
	SoftLightEffect() = default;

	void Update() override
	{
		EVERY_N_MILLIS(EFFECTS_PERIOD_DEFAULT)
		{
			// TODO make it with shelves
			const int width = 15;
			auto leds = m_leds.start;
			for (int i = 0; i < width; ++i)
			{
				CRGB color = GetColor();
				float cosArg = 6.28 * i / width;
				int fadeFactor = 255 * (0.5 + 0.5 * cos(cosArg));
				color.fadeToBlackBy(fadeFactor);
				leds[i] = color;
			}
			for (int i = width; i < width * 4; ++i)
			{
				leds[i] = leds[i % width];
			}
			for (int i = 60; i < 72; ++i)
			{
				leds[i] = leds[width/2];
			}
			FastLED.show();
		}
	}
};
