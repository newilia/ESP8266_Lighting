#include "Effects/EffectsManager.h"
#include "Effects/FadingEffect.h"
#include "Effects/SoftLightEffect.h"
#include "Effects/PlainLightEffect.h"
#include "Utils.h"

void EffectsManager::Init(Leds leds, const SaveData * saveData)
{
	m_leds = leds;
	m_saveData = saveData;
	SetEffect();
}

void EffectsManager::Update()
{
	if (m_currentEffect)
	{
		m_currentEffect->Update();
	}
	else
	{
		EVERY_N_MILLIS(100)
		{
			FastLED.showColor(CRGB::Black);
		}
	}
}

void EffectsManager::SetEffect()
{
	if (m_currentEffect)
	{
		delete m_currentEffect;
		m_currentEffect = nullptr;
	}

	auto effect = m_saveData->currentEffect;
	if (effect == 1) m_currentEffect = new PlainLightEffect(&m_saveData->color);
	if (effect == 2) m_currentEffect = new SoftLightEffect(m_leds, &m_saveData->color);
	if (effect == 3) m_currentEffect = new FadingEffect(m_leds, &m_saveData->color);
}

void EffectsManager::OnEffectChanged()
{
	SetEffect();
}