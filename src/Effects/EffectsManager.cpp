#include "Effects/EffectsManager.h"
#include "Effects/EffectsBase.h"
#include "Effects/FadingEffect.h"
#include "Effects/SoftLightEffect.h"
#include "Effects/PlainLightEffect.h"
#include "Effects/FlasherEffect.h"
#include "Effects/GradientEffect.h"
#include "Utils.h"

void EffectsManager::ConfigureEffect()
{
	LOG_FUNC_LN();
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
		if (m_currentEffectNumber == 5) m_currentEffect = new GradientEffect;
	}

	if (auto effect = GetCurrentEffect<LedsContainer>())
	{
		effect->SetLeds(m_leds);
	}
	if (auto effect = GetCurrentEffect<StripsContainer>())
	{
		effect->SetLedStripsAdapter(m_stripsAdapter);
	}


	if (auto effect = GetCurrentEffect<ColoredEffect>())
	{
		effect->SetColors(m_effectsData->colors, m_effectsData->colorsCount);
	}
	if (auto effect = GetCurrentEffect<SpeedEffect>())
	{
		effect->SetSpeed(m_effectsData->speed);
	}
	if (auto effect = GetCurrentEffect<SizedEffect>())
	{
		effect->SetSize(m_effectsData->size);
	}


	if (auto effect = GetCurrentEffect<FlasherEffect>())
	{
		effect->SetFullWidth(m_effectsData->flasher.fullWidth);
		effect->SetRandomOrder(m_effectsData->flasher.randomOrder);
	}
	if (auto effect = GetCurrentEffect<GradientEffect>())
	{
		effect->SetUniteStripes(m_effectsData->uniteStripes);
		effect->SetDirection(m_effectsData->gradient.direction);
	}
}