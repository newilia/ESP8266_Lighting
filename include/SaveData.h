#pragma once
#include <FastLED.h>
#include <SaveManagerTemplate.h>
#include <ESP8266WiFi.h>

struct SaveData
{
	static const uint8_t CURRENT_VERSION = 10;

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
	struct
	{
		uint8_t 		brightness = 255;
		uint8_t 		currentEffect = 1;
		CRGB			color = CRGB::White;
		uint8_t			speed = EFFECT_SPEED_DEFAULT;
	}				effects;
	
};

using SaveManager = SaveManagerTemplate<SaveData>;