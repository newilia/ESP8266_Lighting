#include "Effects/EffectsManager.h"
#include "Effects/EffectsBase.h"
#include "Effects/FadingEffect.h"
#include "Effects/SoftLightEffect.h"
#include "Effects/PlainLightEffect.h"
#include "Utils.h"

void EffectsManager::Init(Leds leds, const SaveData * saveData)
{
	m_leds = leds;
	m_saveData = saveData;
	ConfigureEffect();
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

void EffectsManager::ConfigureEffect()
{
	if (m_currentEffectNumber != m_saveData->effects.currentEffect)
	{
		if (m_currentEffect)
		{
			delete m_currentEffect;
			m_currentEffect = nullptr;
		}
		m_currentEffectNumber = m_saveData->effects.currentEffect;
		if (m_currentEffectNumber == 1) m_currentEffect = new PlainLightEffect;
		if (m_currentEffectNumber == 2) m_currentEffect = new SoftLightEffect;
		if (m_currentEffectNumber == 3) m_currentEffect = new FadingEffect;
	}

	if (auto effect = GetCurrentEffect<LedsContainer>())
	{
		effect->SetLeds(m_leds);
	}
	if (auto effect = GetCurrentEffect<ColoredEffect>())
	{
		effect->SetColor(m_saveData->effects.color);
	}
	if (auto effect = GetCurrentEffect<SpeedEffect>())
	{
		effect->SetSpeed(m_saveData->effects.speed);
	}
	if (auto effect = GetCurrentEffect<ShelvesEffect>())
	{
		effect->SetShelvesLedsAdapter(m_shelvesAdapter);
	}
}

void EffectsManager::OnEffectSettingsChanged()
{
	ConfigureEffect();
}