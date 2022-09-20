#pragma once
#include <Arduino.h>
#include <utility>
#include <FastLED.h>

#define DEBUG 0
#define time_t unsigned long

//const int LEDS_COUNT = 72;
const int LEDS_PIN = 2;
const int SHELF_WIDTH = 15;

typedef std::pair<CRGB*, int> Leds;