#pragma once
#include "Effects/EffectsBase.h"
#include <FastLED.h>
#include "GlobalDefs.h"

class GradientEffect : public LedsContainer, public ColoredEffect, public SpeedEffect, public StripsContainer, public SizedEffect
{
public:
	enum Direction
	{
		LEFT_TO_RIGHT,
		RIGHT_TO_LEFT,
		TO_CENTER,
		FROM_CENTER
	};

	GradientEffect()
	: ColoredEffect(COLORS_COUNT_MAX, true) {};

	void SetUniteStripes(bool val) { m_uniteStripes = val; }
	void SetDirection(Direction dir) { m_direction = dir; }

	void Update() override
	{
		EVERY_N_MILLIS(EFFECTS_PERIOD_DEFAULT)
		{
			UpdateTime(0.01, 0.1);
			auto size = fmap(m_size, 0, 1, 1, 100);

			//todo use uniteStrips parameter
			for (int j = 0; j < m_stripsAdapter->GetCount(); ++j)
			{
				auto * strip = m_stripsAdapter->GetStrip(j);
				int stripWidth = strip->leds.Count();

				

				for (int i = 0; i < stripWidth; ++i)
				{
					float ledValue = m_time + (float)i / size;
					/*if (m_direction == RIGHT_TO_LEFT || m_direction == FROM_CENTER)
					{
						ledValue = -ledValue;
					}*/

					int colorIndex = abs((int)floor(ledValue) % m_colorsCount);
					int nextColorIndex = (colorIndex + 1) % m_colorsCount;
					float nextColorRatio = ledValue - (int)floor(ledValue);
					strip->leds.start[i] = MixColors(m_colors[nextColorIndex], m_colors[colorIndex], nextColorRatio);
				}
				/*if (m_direction == FROM_CENTER)
				{
					for (int i = 0; i < stripWidth / 2; ++i)
					{
						strip->leds.start[i] = strip->leds.start[stripWidth - 1 - i];
					}
				}
				else if (m_direction == TO_CENTER)
				{
					for (int i = stripWidth / 2; i < stripWidth; ++i)
					{
						strip->leds.start[i] = strip->leds.start[stripWidth - 1 - i];
					}
				}*/
			}
			FastLED.show();
		}
	}
private:
	bool		m_uniteStripes = false;
	Direction	m_direction = LEFT_TO_RIGHT;
};