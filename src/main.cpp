#include "GlobalDefs.h"
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
#include "LedStripsAdapter.h"
#include "SaveData.h"
#include <utility>

LedsRange g_leds;
GyverPortal g_portal;
WebSocketsServer g_socketServer(81);
bool g_isWiFiConnecting = false;

void InitEffectsManager();
void InitWifi();
void InitLeds();
void StartAP();

void setup()
{
	Serial.begin(115200);
	SaveManager::instance();
	
	InitLeds();	
	InitWifi();
	InitPortal();
	InitEffectsManager();
	LOG_LN("Setup finished!");
}

void loop()
{
	g_portal.tick();
	g_socketServer.loop();
	EffectsManager::instance().Update();
	SaveManager::instance().Update();

	if (g_isWiFiConnecting)
	{
		if (WiFi.status() == wl_status_t::WL_CONNECTED)
		{
			g_isWiFiConnecting = false;
			LOG("WiFi connected after");
			LOG(millis());
			LOG("ms, IP:");
			LOG_LN(WiFi.localIP().toString());
		}
		else if (millis() > WIFI_CONNECTING_TIMEOUT)
		{
			g_isWiFiConnecting = false;
			LOG("WiFi connecting timeout, code:");
			LOG_LN((int)WiFi.status());
			StartAP();
		}
	}
}

void InitEffectsManager()
{
	LOG_FUNC_LN();

	if (!g_leds)
	{
		LOG_LN("Failed to init EffectsManager: init leds first");
		return;
	}
	EffectsManager::instance().SetLeds(g_leds);
	EffectsManager::instance().SetData(&SaveManager::instance().GetData().effects);
	EffectsManager::instance().SetStripsCount(1);
	Strip strip;
	strip.leds.start = g_leds.start;
	strip.leds.end = g_leds.end;
	strip.position = 0;
	strip.isSpecial = false;
	EffectsManager::instance().SetStrip(0, strip);
	EffectsManager::instance().Init();
	LOG_LN("Effects manager initialized!");
}

void StartAP()
{
	IPAddress Ip = ADDRESS; 	// IP-адрес для ESP
	IPAddress Gateway(192, 168, 0, 1); 	// IP-адрес шлюза (роутера)
	IPAddress Subnet(255, 255, 255, 0); // маска подсети, диапазон IP-адресов в локальной сети
	if (!WiFi.enableAP(true))
	{
		LOG_LN("Can't enable AP");
	}
	WiFi.softAPConfig(Ip, Gateway, Subnet);

	auto apName = String("АУФ ") + Ip.toString();
	
	if (WiFi.softAP(apName))
	{
		LOG("AP\"");
		LOG(apName);
		LOG_LN("\"started");
	}
	else
	{
		LOG("Can't start AP \"");
		LOG(apName);
		LOG_LN("\"");
	}
}

void ConnectAsStation()
{
	if (WiFi.enableSTA(true))
	{
		LOG_LN("Station enabled");
	}
	else
	{
		LOG_LN("Failed to enable station");
		return;
	}
	IPAddress Ip = ADDRESS; 	// IP-адрес для ESP
	IPAddress Gateway(192, 168, 0, 1); 	// IP-адрес шлюза (роутера)
	IPAddress Subnet(255, 255, 255, 0); // маска подсети, диапазон IP-адресов в локальной сети
	WiFi.config(Ip, Gateway, Subnet);
	auto ssid = "138";
	auto password = "88888888";

	LOG("Connecting: SSID =");
	LOG(ssid);
	LOG(", pass =");
	LOG_LN(password);
	WiFi.begin(ssid, password);
	g_isWiFiConnecting = true;
}

void InitWifi()
{
	WiFi.mode(WiFiMode::WIFI_AP_STA);
	ConnectAsStation();
}

void InitLeds()
{
	auto ledsCount = SaveManager::instance().GetData().ledsCount;
	delete g_leds.start;
	g_leds.start = new CRGB[ledsCount];

	if (g_leds.start)
	{
		g_leds.end = g_leds.start + ledsCount;
		FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(g_leds.start, g_leds.Count());
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