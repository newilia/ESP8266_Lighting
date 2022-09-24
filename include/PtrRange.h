#pragma once
#include <FastLED.h>

template <typename T>
struct PtrRange
{
	T * start = nullptr;
	T * end = nullptr;

	size_t Count() const { return (end - start); }
	operator bool() { return start && end; }
};
