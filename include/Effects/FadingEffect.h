#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include <SaveData.h>

class FadingEffect : public IEffect, public LedsContainer, public SingleColoredEffect
{
public:
	FadingEffect(Leds leds, const CRGB * color)
	: LedsContainer(leds)
	, SingleColoredEffect(color)
	{
	}

	FadingEffect(Leds leds, CRGB color)
	: LedsContainer(leds)
	, SingleColoredEffect(color)
	{
	}

	void Update() override
	{
		EVERY_N_MILLIS(20)
		{
			int brightness = (millis() / 8) % 512;
			if (brightness > 255)
			{
				brightness = 511 - brightness;
			}
			auto color = *m_color;
			color.fadeToBlackBy(brightness);

			for (int i = 0; i < m_leds.second; ++i)
			{
				m_leds.first[i] = color;
			}
			FastLED.show();
		}
	}

};