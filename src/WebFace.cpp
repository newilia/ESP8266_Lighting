#include "WebFace.h"
#include "ExternVars.h"
#include "Effects/EffectsManager.h"
#include "Effects/EffectsBase.h"
#include "Effects/FadingEffect.h"
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
	auto effectShowNames = "Выключить,Ровный свет,Мягкий свет,Затухание";
	auto colorPicker = "color_picker";
	auto effectSpeed = "effect_speed";
	auto fading = "fading";
	auto mainPage = "/";
	auto configPage = "/config";
}

#define data 			SaveManager::instance().GetData()
#define dataToChange 	SaveManager::instance().GetDataToChange()


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

	GP.SELECT(Names::effect, Names::effectShowNames, data.effects.currentEffect);
	
	if (EffectsManager::instance().GetCurrentEffect<ColoredEffect>())
	{
		GP.LABEL(" ");
		GP.COLOR(Names::colorPicker, CRGB_to_int(data.effects.color));
	}
	
	if (EffectsManager::instance().GetCurrentEffect<SpeedEffect>())
	{
		GP.BLOCK_BEGIN();
		GP.SLIDER(Names::effectSpeed, "Скорость", data.effects.speed, EFFECT_SPEED_MIN, EFFECT_SPEED_MAX);
		GP.BLOCK_END();
	}
	
	GP.BREAK();
}

void HandleMainPage()
{
	if (g_portal.click(Names::colorPicker))
	{
		dataToChange.effects.color = g_portal.getColor(Names::colorPicker).getHEX();
		EffectsManager::instance().OnEffectSettingsChanged();
	}
	if (g_portal.click(Names::brightness))
	{
		dataToChange.effects.brightness = g_portal.getInt(Names::brightness);
		FastLED.setBrightness(data.effects.brightness);
		EffectsManager::instance().OnEffectSettingsChanged();
	}
	if (g_portal.click(Names::effect))
	{
		auto effectNumber = g_portal.getSelected(Names::effect, Names::effectShowNames);
		dataToChange.effects.currentEffect = effectNumber;
		EffectsManager::instance().OnEffectSettingsChanged();
		ReloadPage();
	}
	if (g_portal.click(Names::effectSpeed))
	{
		dataToChange.effects.speed = g_portal.getInt(Names::effectSpeed);
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