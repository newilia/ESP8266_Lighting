#include "Effects/EffectsManager.h"
#include "Effects/EffectsBase.h"
#include "Effects/FadingEffect.h"
#include "Effects/SoftLightEffect.h"
#include "Effects/PlainLightEffect.h"
#include "Effects/FlasherEffect.h"
#include "Utils.h"

void EffectsManager::Init(Leds leds, const EffectsData * effectsData)
{
	m_leds = leds;
	m_effectsData = effectsData;
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
	if (m_currentEffectNumber != m_effectsData->currentEffect)
	{
		if (m_currentEffect)
		{
			delete m_currentEffect;
			m_currentEffect = nullptr;
		}
		m_currentEffectNumber = m_effectsData->currentEffect;
		if (m_currentEffectNumber == 1) m_currentEffect = new PlainLightEffect;
		if (m_currentEffectNumber == 2) m_currentEffect = new SoftLightEffect;
		if (m_currentEffectNumber == 3) m_currentEffect = new FadingEffect;
		if (m_currentEffectNumber == 4) m_currentEffect = new FlasherEffect;
	}

	if (auto effect = GetCurrentEffect<LedsContainer>())
	{
		effect->SetLeds(m_leds);
	}
	if (auto effect = GetCurrentEffect<ColoredEffect>())
	{
		effect->SetColors(m_effectsData->colors, m_effectsData->colorsCount);
	}
	if (auto effect = GetCurrentEffect<SpeedEffect>())
	{
		effect->SetSpeed(m_effectsData->speed);
	}
	if (auto effect = GetCurrentEffect<ShelvesEffect>())
	{
		effect->SetShelvesLedsAdapter(m_shelvesAdapter);
	}
	if (auto effect = GetCurrentEffect<FlasherEffect>())
	{
		effect->SetFullWidth(m_effectsData->flasher.fullWidth);
		effect->SetRandomOrder(m_effectsData->flasher.randomOrder);
	}
}

void EffectsManager::OnEffectSettingsChanged()
{
	ConfigureEffect();
}