#pragma once
#include <Arduino.h>

#define LOG(x) Serial.print(x); Serial.print(' ');
#define LOG_LN(x) Serial.println(x);

#define LOG_VAR(x) Serial.print(#x); Serial.print(" = "); Serial.print(x); Serial.print("; ")
#define LOG_VAR_LN(x) Serial.print(#x); Serial.print(" = "); Serial.println(x);

#define LOG_FUNC() Serial.print(__PRETTY_FUNCTION__); Serial.print(' ');
#define LOG_FUNC_LN() Serial.println(__PRETTY_FUNCTION__);

inline void Reboot()
{
	LOG_FUNC_LN();
	int * p = 0;
	*p = 42;
}

inline uint32_t CRGB_to_int(const CRGB & color)
{
	return color.b | color.g << 8 | color.r << 16;
}

inline float fmap(float x, float in_min, float in_max, float out_min, float out_max) 
{
    const float dividend = out_max - out_min;
    const float divisor = in_max - in_min;
    const float delta = x - in_min;
    return (delta * dividend + (divisor / 2)) / divisor + out_min;
}

inline float PowNorm(float value, float maxValue, int power)
{
	float norm = value / maxValue;
	float result = norm;
	for (; power; --power)
	{
		result *= norm;
	}
	return maxValue * result;
}

template<typename T>
inline uint8_t GetTimeSwitchingCounter(time_t time, const T * timings, uint8_t timingsSize, uint8_t counterMax)
{
	time %= timings[timingsSize - 1];
	uint8_t counter = 0;

	for (int i = 0; i < timingsSize; ++i)
	{
		if (timings[i] >= time)
		{
			break;
		}
		++counter;
	}
	uint8_t result = counter % counterMax;
	return result;
}


template<typename T>
inline void Swap(T & val1, T & val2)
{
	T temp = std::move(val1);
	val1 = std::move(val2);
	val2 = std::move(temp);
}

template<typename T>
void EnsureIncreasing(T & val1, T & val2)
{
	if (val1 > val2)
	{
		Swap(val1, val2);
	}
}

inline void ConstrainFloat(float & val, float start, float end)
{
	EnsureIncreasing(start, end);
	if (val < start)
	{
		val = start;
	}
	else if (val > end)
	{
		val = end;
	}
}

inline CRGB MixColors(const CRGB & led1, const CRGB & led2, float ratio)
{
	ConstrainFloat(ratio, 0, 1);
	CRGB result;
	for (int i = 0; i < 3; ++i)
	{
		result[i] = led1[i] * ratio + led2[i] * (1 - ratio);
	}
	return result;
}
