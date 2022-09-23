#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "Utils.h"
#include "GlobalDefs.h"

struct Strip
{
	Strip() = default;
	
	Strip(LedsRange leds, int position, int isOnTop)
	: leds(leds)
	, position(position)
	, isSpecial(isOnTop) {}

	LedsRange	leds;
	float		position = 0;
	bool		isSpecial = false;
};

class LedStripsAdapter
{
	friend class EffectsManager;
public:
	void SetCount(uint8_t stripsCount)
	{
		if (m_strips)
		{
			delete m_strips;
		}
		m_strips = new Strip[stripsCount];
		m_count = stripsCount;
	}

	int GetCount() { return m_count; }

	const Strip * GetStrip(uint8_t index)
	{
		if (index >= m_count || !m_strips)
		{
			return nullptr;
		}
		return m_strips + index;
	}

	void SetStrip(uint8_t index, const Strip & strip)
	{
		if (index >= m_count || !m_strips)
		{
			LOG_LN("SetStrip(): index out of bounds");
			return;
		}
		m_strips[index] = strip;
	}

private:
	LedStripsAdapter() {}
	~LedStripsAdapter() 
	{
		delete m_strips;
	}
	
	Strip *	m_strips;
	int		m_count;
};
