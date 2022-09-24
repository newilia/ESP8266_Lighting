#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include "GlobalDefs.h"

class FlasherEffect : public LedsContainer, public ColoredEffect, public SpeedEffect, public StripsContainer
{
public:

	FlasherEffect()
	: ColoredEffect(COLORS_COUNT_MAX, true) {};

	void SetFullWidth(bool val) { m_isFullWidth = val; }
	bool IsFullWidth() { return m_isFullWidth; }

	void SetRandomOrder(bool val) { m_isRandomOrder = val; }
	bool IsRandomOrder() { return m_isRandomOrder; }

	void Update() override
	{
		UpdateTime(0.1, 4);

		const int flashPeriod = 500;
		const int flashTimings[] = {0,50,100,150,flashPeriod};
		bool isFlash = GetTimeSwitchingCounter(m_time, flashTimings, 5, 2);

		static uint8_t colorCounter = 0;
		auto prevCounter = colorCounter;
		colorCounter = m_time / flashPeriod;
		
		if (prevCounter != colorCounter)
		{
			if (m_isRandomOrder && m_colorsCount > 2)
			{
				m_colorIndex = (m_colorIndex + 1 + rand() % (m_colorsCount - 1)) % m_colorsCount;
			}
			else
			{
				m_colorIndex = colorCounter % m_colorsCount;
			}
		}

		for (int i = 0; i < m_stripsAdapter->GetCount(); ++i)
		{
			auto & strip = *m_stripsAdapter->GetStrip(i);
			auto * flashStart = strip.leds.start;
			auto * flashEnd = strip.leds.end;

			if (!m_isFullWidth && !strip.isSpecial)
			{
				flashStart += strip.leds.Count() * m_colorIndex / m_colorsCount;
				flashEnd = flashStart + (strip.leds.Count() / m_colorsCount);
			}

			for (auto * led = strip.leds.start; led < strip.leds.end; ++led)
			{
				bool isLedInFlashBounds = led >= flashStart && led < flashEnd;
				*led = isFlash && isLedInFlashBounds ? m_colors[m_colorIndex] : CRGB::Black;
			}
		}

		FastLED.show();
	}
	
private:
	bool			m_isFullWidth = true;
	bool			m_isRandomOrder = false;
	uint8_t			m_colorIndex = 0;
};