#include "WebFace.h"
#include "ExternVars.h"
#include "Effects/EffectsManager.h"
#include "Effects/EffectsBase.h"
#include <GyverPortal.h>
#include "Utils.h"
#include "SaveData.h"

namespace Names
{
	auto title = "title";
	auto reset = "reset";
	auto resetData = "reset";
	auto brightness = "brightness";
	auto effect = "effect";
	auto effectShowNames = "Выключить,Ровный свет,Мягкий свет,Затухание";
	auto colorPicker = "color_picker";
	auto fading = "fading";
	auto mainPage = "/";
	auto configPage = "/config";
}

#define data 			SaveManager::instance().GetData()
#define dataToChange 	SaveManager::instance().GetDataToChange()

uint32_t CRGB_to_int(const CRGB & color)
{
	return color.b | color.g << 8 | color.r << 16;
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
	GP.LABEL("Яркость", "");
	GP.BREAK();
	GP.SLIDER(Names::brightness, data.brightness, 0, 255);
	GP.BREAK();
	GP.BLOCK_END();

	GP.SELECT(Names::effect, Names::effectShowNames, data.currentEffect);
	GP.LABEL(" ");
	GP.COLOR(Names::colorPicker, CRGB_to_int(data.color));
	GP.BREAK();
}

void HandleMainPage()
{
	if (g_portal.click(Names::colorPicker))
	{
		dataToChange.color = g_portal.getColor(Names::colorPicker).getHEX();
	}
	if (g_portal.click(Names::brightness))
	{
		dataToChange.brightness = g_portal.getInt(Names::brightness);
		FastLED.setBrightness(data.brightness);
	}
	if (g_portal.click(Names::effect))
	{
		int effect = g_portal.getSelected(Names::effect, Names::effectShowNames);
		dataToChange.currentEffect = effect;
		EffectsManager::instance().OnEffectChanged();
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
		Reboot();
	}
	else if (g_portal.click(Names::resetData))
	{
		SaveManager::instance().ResetAndSave();
		g_portal.server.send(200, "text/plain", "reload");
		Reboot();
	}
}

void AddScript()
{
	
}

#undef data
#undef dataToChange