#pragma once
#include <FastLED.h>
#include "GlobalDefs.h"
#include "ShelvesLedsAdapter.h"
#include "Utils.h"

class IEffect
{
public:
	virtual ~IEffect() {}
	virtual void Update() = 0;
};

class LedsContainer : virtual public IEffect
{
protected:
	Leds	m_leds = {nullptr, 0};
public:
	LedsContainer() = default;
	void SetLeds(Leds leds) { m_leds = leds; }
};

class ColoredEffect : virtual public IEffect
{
protected:
	CRGB			m_colors[COLORS_COUNT_MAX];
	uint8_t			m_colorsCount = 1;
	const bool		m_canAdjustColorsCount;
	const uint8_t	m_maxColorCount;

private:
	void SetColorsCount(uint8_t count)
	{
		count = min(count, m_maxColorCount);

		/*LOG("Reallocating colors from");
		LOG(m_colorsCount);
		LOG("to");
		LOG_LN(count);
		
		auto newColors = new CRGB[count];
		if (!newColors)
		{
			LOG_LN("Fatal error: failed to reallocate colors");
			return;
		}

		LOG("memcpy...");
		memcpy(newColors, m_colors, m_colorsCount * sizeof(CRGB));
		LOG("memset...");
		memset(newColors + m_colorsCount, 0, sizeof(CRGB) * (count - m_colorsCount));
		LOG("delete m_colors...");
		delete m_colors;

		m_colors = newColors;*/
		m_colorsCount = count;
		//LOG_LN("Ok");
	}
public:
	ColoredEffect(uint8_t maxColorCount = 1, bool canAdjustColorsCount = false)
	: m_canAdjustColorsCount(canAdjustColorsCount)
	, m_maxColorCount(maxColorCount) {};

	~ColoredEffect()
	{
		//delete m_colors;
	}

	constexpr bool CanAdjustColorsCount() { return m_canAdjustColorsCount; }
	constexpr uint8_t GetMaxColorCount() { return m_maxColorCount; }

	void SetColor(CRGB color, uint8_t index = 0) 
	{
		if (index < 0 || index >= COLORS_COUNT_MAX)
		{
			return;
		}
		m_colors[index] = color;
	}

	void SetColors(const CRGB * colors, uint8_t count)
	{
		LOG_FUNC();
		if (m_colorsCount != count)
		{
			SetColorsCount(count);
		}
		for (int i = 0 ; i < m_colorsCount; ++i)
		{
			m_colors[i] = colors[i];
			LOG(i);
		}
		LOG_LN("Colors set");
	}

	CRGB GetColor(uint8_t index = 0)
	{
		if (index < 0 || index >= m_colorsCount)
		{
			return CRGB();
		}
		return m_colors[index];
	}

	uint8_t GetColorsCount() { return m_colorsCount; }
};

class SpeedEffect : virtual public IEffect
{
protected:
	float m_speed = EFFECT_SPEED_DEFAULT;

public:
	SpeedEffect()
	{
		SetSpeed(EFFECT_SPEED_DEFAULT);
	}

	void SetSpeed(float speed) 
	{
		LOG_FUNC();
		LOG_VAR_LN(speed);
		m_speed = speed;
	}
	float GetSpeed() { return m_speed; }
};

class ShelvesEffect : virtual public IEffect
{
protected:
	ShelvesLedsAdapter * m_adapter;
public:
	ShelvesEffect() = default;
	void SetShelvesLedsAdapter(ShelvesLedsAdapter & adapter) { m_adapter = &adapter; }
};