#pragma once
#include <FastLED.h>
#include "ExternVars.h"
#include <SaveData.h>

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
	CRGB 	m_color = CRGB::White;
public:
	ColoredEffect() = default;
	void SetColor(CRGB color) {m_color = color; }
	CRGB GetColor()	{ return m_color; }
};

const uint8_t PERIOD_MIN = 5;
const uint8_t PERIOD_MAX = 30;

class SpeedEffect : virtual public IEffect
{
protected:
	uint8_t m_speed = EFFECT_SPEED_DEFAULT;
	uint8_t m_defaultUpdatePeriod = 0;

public:
	SpeedEffect()
	{
		SetSpeed(EFFECT_SPEED_DEFAULT);
	}

	void SetSpeed(uint8_t speed) 
	{
		m_speed = speed;
		m_defaultUpdatePeriod = fmap(m_speed, EFFECT_SPEED_MIN, EFFECT_SPEED_MAX, PERIOD_MAX, PERIOD_MIN);
	}
	uint8_t GetSpeed() { return m_speed; }
};

class ShelvesEffect : virtual public IEffect
{
protected:
	ShelvesLedsAdapter	m_adapter;
public:
	ShelvesEffect() = default;
	void SetShelvesLedsAdapter(ShelvesLedsAdapter adapter) { m_adapter = adapter; }
};