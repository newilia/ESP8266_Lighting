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