#pragma once
#include <FastLED.h>
#include <SaveManagerTemplate.h>
#include <ESP8266WiFi.h>
#include <Effects/FlasherEffect.h>

struct SaveData
{
	static const uint8_t CURRENT_VERSION = 13;

	uint8_t 		version = CURRENT_VERSION;
	struct
	{
		WiFiMode		mode = WiFiMode::WIFI_STA;
		wl_status_t 	connectionStatus;
		char			SSID[32] = "138";
		char			password[32] = "88888888";
		char			ApName[32] = "АУФ";
	}				wifi;

	int				ledsCount = 72;

	struct EffectsData
	{
		uint8_t 		brightness = 255;
		uint8_t 		currentEffect = 1;
		CRGB			colors[COLORS_COUNT_MAX] = {CRGB::White, CRGB::Blue, CRGB::Red};
		float			speed = EFFECT_SPEED_DEFAULT;
		uint8_t			colorsCount = 1;
		struct 
		{
			bool			fullWidth = true;
			bool			randomOrder = false;
		}				flasher;
			
	}				effects;
	
};

using SaveManager = SaveManagerTemplate<SaveData>;