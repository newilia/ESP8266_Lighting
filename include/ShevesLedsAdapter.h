#pragma once
#include <Arduino.h>
#include <FastLED.h>

struct Shelf
{
	Shelf() = default;
	
	Shelf(CRGB * start, int size, int height, int isOnTop)
	: start(start)
	, size(size)
	, height(height)
	, isOnTop(isOnTop) {}

	CRGB * 	start = nullptr;
	int		size = 0;
	float	height = 0;
	bool	isOnTop = false;
};

class ShelvesLedsAdapter
{
private:
	Shelf *	m_shelves;
	int		m_count;

public:
	ShelvesLedsAdapter() {}
	~ShelvesLedsAdapter() 
	{
		delete m_shelves;
	}

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

	const Shelf * GetShelf(uint8_t index) const
	{
		if (index >= m_count || !m_shelves)
		{
			return nullptr;
		}
		return &m_shelves[index];
	}
};
