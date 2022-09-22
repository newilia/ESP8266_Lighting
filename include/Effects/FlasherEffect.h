#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include "GlobalDefs.h"

class FlasherEffect : public LedsContainer, public ColoredEffect, public SpeedEffect, public ShelvesEffect
{
public:
	enum Mode
	{
		HALF,
		FULL,
		COUNT
	};

	FlasherEffect()
	: ColoredEffect(COLORS_COUNT_MAX, true) {};

	void SetMode(Mode mode) { m_mode = mode; }
	Mode GetMode() { return m_mode; }

	void Update() override
	{
		EVERY_N_MILLIS(EFFECTS_PERIOD_DEFAULT)
		{
			m_time += millis() * (0.1 + m_speed);

			const int flashCount = 2;
			bool flash = squarewave8(m_time);
			//bool pause = squarewave8()

			/*const float periodScale = fmap(m_speed, 0, 1, 1, 0.1);

			
			const int flashDuration = 100 * periodScale;
			const int flashInterval = 100 * periodScale;
			const int pauseDuration = 200 * periodScale;

			const int flashPeriod = flashDuration + flashInterval;
			//const int pauseSwitchPeriod = flashPeriod * flashCount + pauseDuration;
			const int colorSwitchPeriod = flashCount * flashPeriod + pauseDuration;
			const int totalPeriod = m_colorsCount * colorSwitchPeriod;

			static bool ledsOn = true;
			EVERY_N_MILLIS_I(flashTimer, flashDuration)
			{
				ledsOn = !ledsOn;
				flashTimer = flashDuration;
			}

			static bool isPause = false;
			EVERY_N_MILLIS_I(pauseTimer, pauseDuration)
			{
				isPause = !isPause;
				pauseTimer = pauseSwitchPeriod;
			}

			static uint8_t colorIndex = 0;
			EVERY_N_MILLIS_I(colorTimer, colorSwitchPeriod)
			{
				colorIndex = (colorIndex + 1) % m_colorsCount;
				colorTimer = colorSwitchPeriod;
			}*/

			auto shelves = m_adapter->GetShelves();
			
			for (int i = 0; i < shelves.second; ++i)
			{
				auto shelf = shelves.first[i];
				int start = m_mode == FULL ? 0 : (shelf.size * colorIndex / m_colorsCount);
				int end = m_mode == FULL ? shelf.size : (shelf.size * (colorIndex + 1) / m_colorsCount);

				for (int j = 0; j < shelf.size; ++j)
				{
					LOG_VAR_LN(j);
					shelf.leds[j] = !isPause && ledsOn && j >= start && j < end ? m_colors[colorIndex] : CRGB::Black;
				}				
			}

			FastLED.show();
		}
	}
	
private:
	Mode			m_mode = HALF;
	uint32_t		m_time = 0;
};