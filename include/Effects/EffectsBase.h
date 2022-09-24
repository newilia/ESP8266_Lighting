#pragma once
#include <FastLED.h>
#include "GlobalDefs.h"
#include "LedStripsAdapter.h"
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
	LedsRange	m_leds;
public:
	LedsContainer() = default;
	void SetLeds(LedsRange leds) { m_leds = leds; }
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
		m_colorsCount = count;
	}
public:
	ColoredEffect(uint8_t maxColorCount = 1, bool canAdjustColorsCount = false)
	: m_canAdjustColorsCount(canAdjustColorsCount)
	, m_maxColorCount(maxColorCount) {};

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
		if (m_colorsCount != count)
		{
			SetColorsCount(count);
		}
		for (int i = 0 ; i < m_colorsCount; ++i)
		{
			m_colors[i] = colors[i];
		}
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
	float 		m_speed = EFFECT_SPEED_DEFAULT;
	float		m_time = 0;

	void UpdateTime(float minSpeed, float speedScale)
	{
		m_time += speedScale * (minSpeed + m_speed);
	}

public:
	SpeedEffect() = default;
	void SetSpeed(float speed) {m_speed = speed; }
	float GetSpeed() { return m_speed; }
};

class StripsContainer : virtual public IEffect
{
protected:
	LedStripsAdapter * m_stripsAdapter;
public:
	StripsContainer() = default;
	void SetLedStripsAdapter(LedStripsAdapter & adapter) { m_stripsAdapter = &adapter; }
};

class SizedEffect : virtual public IEffect
{
protected:
	float	m_size = 0.5;
public:
	void SetSize(float size) { m_size = size; }
	float GetSize() { return m_size; }
};