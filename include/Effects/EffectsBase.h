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

class LedsContainer
{
public:
	LedsContainer(Leds leds)
	: m_leds(leds) {}

protected:
	Leds	m_leds;
};

class SingleColoredEffect
{
public:
	SingleColoredEffect(const CRGB * externalColor)
	: m_color(externalColor) {}

	SingleColoredEffect(CRGB externalColor)
	: m_color(new CRGB(externalColor))
	, m_isColorOwner(true) {}

	~SingleColoredEffect()
	{
		if (m_isColorOwner)
		{
			delete m_color;
		}
	}

protected:
	const CRGB * 	m_color = nullptr;
	bool			m_isColorOwner = false;
};

template <typename TParam>
class ParametrizedEffect
{
public:
	ParametrizedEffect(const TParam * param)
	: m_param(param) {}

protected:
	const TParam * m_param = nullptr;
};