#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include <utility>

struct Shelf
{
	Shelf() = default;
	
	Shelf(CRGB * leds, int size, int height, int isOnTop)
	: leds(leds)
	, size(size)
	, height(height)
	, isOnTop(isOnTop) {}

	CRGB * 	leds = nullptr;
	int		size = 0;
	float	height = 0;
	bool	isOnTop = false;
};

class ShelvesLedsAdapter
{
	friend class EffectsManager;
public:
	void Init(uint8_t shelvesCount)
	{
		m_shelves = new Shelf[shelvesCount];
		m_count = shelvesCount;
	}

	void SetShelf(uint8_t index, const Shelf & shelf)
	{
		if (index >= m_count || !m_shelves)
		{
			return;
		}
		m_shelves[index] = shelf;
	}

	std::pair<const Shelf*, int> GetShelves() const
	{
		return {m_shelves, m_count};
	}

private:
	ShelvesLedsAdapter() {}
	~ShelvesLedsAdapter() 
	{
		delete m_shelves;
	}

	
	Shelf *	m_shelves;
	int		m_count;

};
