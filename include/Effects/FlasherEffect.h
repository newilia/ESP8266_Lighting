#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include "GlobalDefs.h"

class FlasherEffect : public LedsContainer, public ColoredEffect, public SpeedEffect, public ShelvesEffect
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
		const uint8_t speedScale = 4;
		m_time += speedScale * (0.1 + m_speed);

		const int flashPeriod = 300;
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

		auto shelves = m_adapter->GetShelves();
		for (int i = 0; i < shelves.second; ++i)
		{
			auto shelf = shelves.first[i];
			int start = m_isFullWidth ? 0 : (shelf.size * m_colorIndex / m_colorsCount);
			int end = m_isFullWidth ? shelf.size : (shelf.size * (m_colorIndex + 1) / m_colorsCount);

			for (int j = 0; j < shelf.size; ++j)
			{
				shelf.leds[j] = isFlash && j >= start && j < end ? m_colors[m_colorIndex] : CRGB::Black;
			}
		}

		FastLED.show();
	}
	
private:
	bool			m_isFullWidth = true;
	bool			m_isRandomOrder = false;
	float			m_time = 0;
	uint8_t			m_colorIndex = 0;
};