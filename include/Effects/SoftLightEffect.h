#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include <Utils.h>

class SoftLightEffect : public LedsContainer, public ColoredEffect, public StripsContainer
{
public:
	SoftLightEffect() = default;

	void Update() override
	{
		EVERY_N_MILLIS(EFFECTS_PERIOD_DEFAULT)
		{
			for (int j = 0; j < m_stripsAdapter->GetCount(); ++j)
			{
				auto * strip = m_stripsAdapter->GetStrip(j);
				int width = strip->leds.Count();
				for (int i = 0; i < width; ++i)
				{
					CRGB color = GetColor();
					float cosArg = 6.28 * i / width;
					int fadeFactor = 255 * (0.5 + 0.5 * cos(cosArg));
					color.fadeToBlackBy(fadeFactor);
					strip->leds.start[i] = color;
				}
			}
			FastLED.show();
		}
	}
};
