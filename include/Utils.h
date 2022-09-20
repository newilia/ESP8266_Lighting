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