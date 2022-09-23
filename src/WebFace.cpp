#include "WebFace.h"
#include "ExternVars.h"
#include "Effects/EffectsManager.h"
#include "Effects/EffectsBase.h"
#include "Effects/FadingEffect.h"
#include "Effects/FlasherEffect.h"
#include <GyverPortal.h>
#include "Utils.h"
#include "SaveData.h"
#include <WebSocketsServer.h>

namespace Names
{
	auto title = "title";
	auto reset = "reset";
	auto resetData = "resetData";
	auto brightness = "brightness";
	auto effect = "effect";
	auto effectShowNames = "Выключить,Ровный свет,Мягкий свет,Затухание,Стробоскоп";
	auto colorPickerPrefix = "color_picker";
	auto effectSpeed = "effect_speed";
	auto fading = "fading";
	auto mainPage = "/";
	auto configPage = "/config";
	auto colorsCount = "colors_count";
	auto flasherFullWidth = "flasher_full_width";
	auto flasherRandomOrder = "flasher_random_order";
	auto ledsCount = "leds_count";
	auto applySettings = "apply_settings";
	auto wifiMode = "wifi_mode";
	auto wifiModeNames = "Подкл. к роутеру,Точка доступа";
	//todo
	auto ssid = "ssid";
	auto password = "password";
	auto apName = "apName";
}

namespace TempVals
{
	int 		ledsCount = 0;
	WiFiMode 	wifiMode;
	char		ssid[32] = "";
	char		password[32] = "";
	char		apName[32] = "";
}

#define data 			SaveManager::instance().GetData()
#define dataToChange 	SaveManager::instance().GetDataToChange()

const int SPEED_SLIDER_MAX = 10;

void SendMessage(const char * message)
{
	g_socketServer.broadcastTXT(message);
}

void ReloadPage()
{
	LOG_FUNC_LN();
	SendMessage("reload_page");
}

void InitPortal()
{
	g_portal.attachBuild(BuildPage);
	g_portal.attach(ActionHandler);
	g_portal.start();
}

void BuildPage()
{
	BUILD_BEGIN();
	GP.THEME(GP_DARK);
	if (g_portal.uri() == Names::mainPage)
	{
		BuildMainPage();
	}
	else if (g_portal.uri() == Names::configPage)
	{
		BuildConfigPage();
	}
	AddCustomScript();
	BUILD_END();
	LOG_LN(g_portal.uri());
}

void ActionHandler()
{	
	if (g_portal.click())
	{
		LOG(g_portal.clickName());
		LOG_LN("clicked");
		HandleMainPage();
		HandleConfigPage();
	}
}

void BuildMainPage()
{
	GP.TITLE("Хэй йоу", Names::title);
	GP.BUTTON_LINK(Names::configPage, "Настройки");
	GP.HR();
	
	GP.BLOCK_BEGIN();
	GP.SLIDER(Names::brightness, "Яркость", data.effects.brightness, 0, 255);
	GP.BLOCK_END();

	GP.BLOCK_BEGIN();
	GP.LABEL("Режим");
	GP.SELECT(Names::effect, Names::effectShowNames, data.effects.currentEffect);
	GP.BREAK();
	GP.BLOCK_END();

	if (auto effect = EffectsManager::instance().GetCurrentEffect<ColoredEffect>())
	{
		GP.BLOCK_BEGIN();
		GP.LABEL("Цвет");
		if (effect->CanAdjustColorsCount())
		{
			GP.NUMBER(Names::colorsCount, "", data.effects.colorsCount, 1, effect->GetMaxColorCount());
		}
		for (int i = 0; i < effect->GetColorsCount(); ++i)
		{
			auto colorPickerName = String(Names::colorPickerPrefix) + String(i);
			GP.COLOR(colorPickerName.c_str(), CRGB_to_int(data.effects.colors[i]));
		}
		GP.BLOCK_END();
	}
	
	if (EffectsManager::instance().GetCurrentEffect<SpeedEffect>())
	{
		GP.BLOCK_BEGIN();
		int sliderValue = data.effects.speed * SPEED_SLIDER_MAX;
		GP.SLIDER(Names::effectSpeed, "Скорость", sliderValue, 0, SPEED_SLIDER_MAX);
		GP.BLOCK_END();
	}

	if (auto effect = EffectsManager::instance().GetCurrentEffect<FlasherEffect>())
	{
		if (effect->GetColorsCount() > 1 || effect->GetColorsCount() > 2)
		{
			GP.BLOCK_BEGIN();
			if (effect->GetColorsCount() > 1)
			{
				GP.LABEL("Вся ширина");
				GP.CHECK(Names::flasherFullWidth, data.effects.flasher.fullWidth);
			}
			if (effect->GetColorsCount() > 2)
			{
				GP.LABEL("Случайный порядок");
				GP.CHECK(Names::flasherRandomOrder, data.effects.flasher.randomOrder);
			}
			GP.BLOCK_END();
		}
	}
}

void HandleMainPage()
{
	if (auto clickedElement = g_portal.clickName();
		clickedElement.indexOf(Names::colorPickerPrefix) != -1)
	{
		uint8_t index = clickedElement[strlen(Names::colorPickerPrefix)] - '0';
		if (index < 0 || index >= COLORS_COUNT_MAX)
		{
			LOG("Invalid color index received: ");
			LOG_LN(index);
			return;
		}
		dataToChange.effects.colors[index] = g_portal.getColor(clickedElement).getHEX();
		EffectsManager::instance().OnEffectSettingsChanged();
	}
	else if (g_portal.click(Names::colorsCount))
	{
		dataToChange.effects.colorsCount = g_portal.getInt(Names::colorsCount);
		EffectsManager::instance().OnEffectSettingsChanged();
		ReloadPage();
	}
	else if (g_portal.click(Names::brightness))
	{
		dataToChange.effects.brightness = g_portal.getInt(Names::brightness);
		FastLED.setBrightness(data.effects.brightness);
		EffectsManager::instance().OnEffectSettingsChanged();
	}
	else if (g_portal.click(Names::effect))
	{
		auto effectNumber = g_portal.getSelected(Names::effect, Names::effectShowNames);
		dataToChange.effects.currentEffect = effectNumber;
		EffectsManager::instance().OnEffectSettingsChanged();
		ReloadPage();
	}
	else if (g_portal.click(Names::effectSpeed))
	{
		dataToChange.effects.speed = (float)g_portal.getInt(Names::effectSpeed) / SPEED_SLIDER_MAX;
		EffectsManager::instance().OnEffectSettingsChanged();
	}
	else if (g_portal.click(Names::flasherFullWidth))
	{
		dataToChange.effects.flasher.fullWidth = g_portal.getCheck(Names::flasherFullWidth);
		EffectsManager::instance().OnEffectSettingsChanged();
	}
	else if (g_portal.click(Names::flasherRandomOrder))
	{
		dataToChange.effects.flasher.randomOrder = g_portal.getCheck(Names::flasherRandomOrder);
		EffectsManager::instance().OnEffectSettingsChanged();
	}
}

void BuildConfigPage()
{
	GP.TITLE("Настройки", Names::title);
	GP.HR();

	GP.BUTTON_LINK(Names::mainPage, "Назад");
	GP.BREAK();

	GP.BUTTON(Names::reset, "Перезагрузить");
	GP.BREAK();

	GP.HR();
	{
		GP.LABEL("Кол-во светодиодов");
		TempVals::ledsCount = data.ledsCount;
		GP.NUMBER(Names::ledsCount, "", data.ledsCount, 1, 300);
		GP.BREAK();

		if (data.wifi.connectionStatus == wl_status_t::WL_NO_SSID_AVAIL)
		{
			GP.LABEL("Не удалось подкючиться: сеть с выбранным SSID не найдена");
			GP.BREAK();
		}
		else if (data.wifi.connectionStatus == wl_status_t::WL_WRONG_PASSWORD)
		{
			GP.LABEL("Не удалось подкючиться: неверный пароль");
			GP.BREAK();
		}
		else if (data.wifi.connectionStatus != wl_status_t::WL_CONNECTED)
		{
			String str = String("Не удалось подкючиться: код ошибки ") + String(int(data.wifi.connectionStatus));
			GP.LABEL(str.c_str());
			GP.BREAK();
		}

		GP.LABEL("Режим WiFi");
		TempVals::wifiMode = data.wifi.mode;
		GP.SELECT(Names::wifiMode, Names::wifiModeNames, (int)data.wifi.mode - 1);
		GP.BREAK();

		if (data.wifi.mode == WiFiMode::WIFI_AP)
		{
			GP.LABEL("Имя точки доступа");
			strcpy(TempVals::apName, data.wifi.ApName);
			GP.TEXT(Names::apName, "", data.wifi.ApName);
			GP.BREAK();
		}
		else if (data.wifi.mode == WiFiMode::WIFI_STA)
		{
			GP.LABEL("SSID");
			strcpy(TempVals::ssid, data.wifi.SSID);
			GP.TEXT(Names::ssid, "", data.wifi.SSID);
			GP.BREAK();
			GP.LABEL("Пароль");
			strcpy(TempVals::password, data.wifi.password);
			GP.TEXT(Names::password, "", data.wifi.password);
			GP.BREAK();
		}

		GP.BUTTON(Names::applySettings, "Применить");
	}
	GP.HR();

	GP.BUTTON(Names::resetData, "Сбросить установки");
	GP.BREAK();
}

void HandleConfigPage()
{
	if (g_portal.click(Names::reset))
	{
		ReloadPage();
		Reboot();
	}
	else if (g_portal.click(Names::resetData))
	{
		ReloadPage();
		SaveManager::instance().ResetAndSave();
		Reboot();
	}
	else if (g_portal.click(Names::ledsCount))
	{
		TempVals::ledsCount = g_portal.getInt(Names::ledsCount);
	}
	else if (g_portal.click(Names::wifiMode))
	{
		TempVals::wifiMode = (WiFiMode)(g_portal.getSelected(Names::wifiMode, Names::wifiModeNames) + 1);
	}
	else if (false)
	{
		// todo get apName, ssid, password
	}
	else if (g_portal.click(Names::applySettings))
	{
		dataToChange.ledsCount = TempVals::ledsCount;
		dataToChange.wifi.mode = TempVals::wifiMode;
		if (strlen(TempVals::apName))
		{
			strcpy(dataToChange.wifi.ApName, TempVals::apName);
		}
		if (strlen(TempVals::ssid))
		{
			strcpy(dataToChange.wifi.SSID, TempVals::ssid);
		}
		if (strlen(TempVals::password))
		{
			strcpy(dataToChange.wifi.password, TempVals::password);
		}		
		SaveManager::instance().Save();
		Reboot();
	}
}

void AddCustomScript()
{
	*_GP += 
	F("<script type=\"text/javascript\">\\
	const socket = new WebSocket('ws://' + window.location.hostname + ':81/');\\
	socket.addEventListener('message', (event) => {\\
		console.log('Message from server ', event.data);\\
		\\
		if (event.data == 'reload_page') {\\
			window.location.reload();\\
		}\\
	});\\
	</script>");
}

#undef data
#undef dataToChange