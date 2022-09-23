#pragma once
#include <FastLED.h>

template <typename T>
struct PtrRange
{
	T * start = nullptr;
	T * end = nullptr;

	int Size() const { return (end - start); }
	operator bool() { return start && end; }
};
