#include "GlobalDefs.h"
#define DEBUG_WIFI(x) Serial.println(x);
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <FastLED.h>
#include "ServerUtils.h"
#include <GyverPortal.h>
#include "WebFace.h"
#include "Effects/EffectsBase.h"
#include "Effects/EffectsManager.h"
#include "Effects/FadingEffect.h"
#include "ShelvesLedsAdapter.h"
#include "SaveData.h"
#include <utility>

Leds g_leds;
GyverPortal g_portal;
WebSocketsServer g_socketServer(81);
void InitEffectsManager();
void InitWifi();
void InitLeds();

void setup()
{
	Serial.begin(115200);
	SaveManager::instance();
	
	InitLeds();	
	InitWifi();
	InitPortal();
	InitEffectsManager();
	g_socketServer.begin();
}

void loop()
{
	g_portal.tick();
	g_socketServer.loop();
	EffectsManager::instance().Update();
	SaveManager::instance().Update();
}

void InitEffectsManager()
{
	if (!g_leds.first)
	{
		LOG_LN("Failed to init EffectsManager: init leds first");
		return;
	}

	EffectsManager::instance().Init(g_leds, &SaveManager::instance().GetData().effects);
	
	auto & adapter = EffectsManager::instance().GetShelvesAdapter();
	adapter.Init(4);
	float heights[] = {0, 0.33, 0.5, 1};
	for (int i = 0; i < 4; ++i)
	{
		CRGB * ledsStart = g_leds.first + i * SHELF_WIDTH;
		CRGB * ledsEnd = ledsStart + SHELF_WIDTH;
		if (ledsEnd - g_leds.first > g_leds.second)
		{
			LOG_LN("Shelf out of bounds");
		}
		adapter.SetShelf(i, Shelf(ledsStart, SHELF_WIDTH, heights[i], false));
	}
	adapter.SetShelf(4, Shelf(g_leds.first + 4 * SHELF_WIDTH, 6, heights[3], true));
}

void EnableAP()
{
	IPAddress Ip(192, 168, 0, 123); 	// IP-адрес для ESP
	IPAddress Gateway(192, 168, 0, 1); 	// IP-адрес шлюза (роутера)
	IPAddress Subnet(255, 255, 255, 0); // маска подсети, диапазон IP-адресов в локальной сети
	if (!WiFi.enableAP(true))
	{
		LOG_LN("Can't enable AP");
	}
	WiFi.softAPConfig(Ip, Gateway, Subnet);

	auto apName = SaveManager::instance().GetData().wifi.ApName;
	
	if (WiFi.softAP(apName))
	{
		LOG("AP");
		LOG(apName);
		LOG_LN("started");
	}
	else
	{
		LOG_LN("Can't start AP");
	}
}

void InitWifi()
{
	const auto & param = SaveManager::instance().GetData().wifi;
	
	if (param.mode & WiFiMode::WIFI_STA)
	{
		IPAddress Ip(192, 168, 0, 123); 	// IP-адрес для ESP
		IPAddress Gateway(192, 168, 0, 1); 	// IP-адрес шлюза (роутера)
		IPAddress Subnet(255, 255, 255, 0); // маска подсети, диапазон IP-адресов в локальной сети
		WiFi.config(Ip, Gateway, Subnet);

		WiFi.mode(param.mode);
		WiFi.begin(param.SSID, param.password);

		Serial.print("Connecting: SSID = ");
		Serial.print(param.SSID);
		Serial.print(", pass = ");
		Serial.println(param.password);

		
		FadingEffect effect;
		effect.SetLeds(g_leds);
		effect.SetColor(CRGB::Orange);
		effect.SetSpeed(1.2);
		while (WiFi.status() == WL_DISCONNECTED)
		{
			effect.Update();
			delay(10);
		}

		SaveManager::instance().GetDataToChange().wifi.connectionStatus = WiFi.status();

		if (WiFi.isConnected())
		{
			Serial.print("Connected. IP address: ");
			Serial.println(WiFi.localIP());
		}
		else
		{		
			for (int i = 0; i < 3; ++i)
			{
				FastLED.showColor(CRGB::Black);
				delay(300);
				FastLED.showColor(CRGB::Red);
				delay(300);
			}
			FastLED.showColor(CRGB::Black);
			//SaveManager::instance().GetDataToChange().wifi.mode = WiFiMode::WIFI_AP;
		}
	}

	if (param.mode & WiFiMode::WIFI_AP)
	{
		EnableAP();
	}
}

void InitLeds()
{
	auto ledsCount = SaveManager::instance().GetData().ledsCount;
	g_leds.first = new CRGB[ledsCount];

	if (g_leds.first)
	{
		g_leds.second = ledsCount;
		FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(g_leds.first, g_leds.second);
		FastLED.setTemperature(CRGB(255, 190, 140));
		FastLED.setDither(DISABLE_DITHER);
		FastLED.showColor(CRGB::Black);
		FastLED.setBrightness(SaveManager::instance().GetData().effects.brightness);
	}
	else
	{
		LOG_LN("Failed to allocate g_leds");
	}	
}