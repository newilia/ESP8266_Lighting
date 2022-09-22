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
	auto resetData = "reset";
	auto brightness = "brightness";
	auto effect = "effect";
	auto effectShowNames = "Выключить,Ровный свет,Мягкий свет,Затухание,Стробоскоп";
	auto colorPickerPrefix = "color_picker";
	auto effectSpeed = "effect_speed";
	auto fading = "fading";
	auto mainPage = "/";
	auto configPage = "/config";
	auto colorsCount = "colors_count";
	auto flasherMode = "flasher_mode";
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

	if (EffectsManager::instance().GetCurrentEffect<FlasherEffect>())
	{
		GP.BLOCK_BEGIN();
		int mode = (int) data.effects.flasherMode;
		GP.SLIDER(Names::flasherMode, "Режим", mode, 0, (int)FlasherEffect::Mode::COUNT - 1);
		GP.BLOCK_END();
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
	else if (g_portal.click(Names::flasherMode))
	{
		dataToChange.effects.flasherMode = (FlasherEffect::Mode) g_portal.getInt(Names::flasherMode);
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