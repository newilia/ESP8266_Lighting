#pragma once
#include <FastLED.h>
#include <SaveManagerTemplate.h>
#include <ESP8266WiFi.h>
#include <Effects/FlasherEffect.h>
#include <Effects/GradientEffect.h>

struct SaveData
{
	static const uint8_t CURRENT_VERSION = 20;

	uint8_t 		version = CURRENT_VERSION;
	/*struct
	{
		WiFiMode		mode = WiFiMode::WIFI_STA;
		wl_status_t 	connectionStatus;
		char			SSID[32] = "138";
		char			password[32] = "88888888";
		char			ApName[32] = "АУФ";
	}				wifi;*/

	int				ledsCount = 72;

	struct EffectsData
	{
		uint8_t 		brightness = 255;
		uint8_t 		currentEffect = 1;
		CRGB			colors[COLORS_COUNT_MAX] = {CRGB::Red, CRGB(0xFF6000), CRGB::Yellow, CRGB::ForestGreen, CRGB::Blue, CRGB::Purple};
		float			speed = EFFECT_SPEED_DEFAULT;
		uint8_t			colorsCount = 1;
		float			size = 0.5;
		bool			uniteStripes = false;
		struct
		{
			bool			fullWidth = true;
			bool			randomOrder = false;
		}				flasher;
		struct
		{
			GradientEffect::Direction	direction = GradientEffect::Direction::LEFT_TO_RIGHT;
		}				gradient;
	}				effects;
	
};

using SaveManager = SaveManagerTemplate<SaveData>;