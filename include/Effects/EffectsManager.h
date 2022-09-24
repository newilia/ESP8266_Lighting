#pragma once
#include <FastLED.h>
#include "Effects/EffectsBase.h"
#include "Singleton.h"
#include "LedStripsAdapter.h"
#include "SaveData.h"

using EffectsData = SaveData::EffectsData;

class EffectsManager : public Singleton<EffectsManager>
{
	friend class Singleton<EffectsManager>;
public:
	void SetLeds(LedsRange leds) { m_leds = leds; }

	void SetData(const EffectsData * data) { m_effectsData = data; }
	
	void SetStripsCount(int count)
	{
		m_stripsAdapter.SetCount(count);
	}

	int GetStripesCount()
	{
		return m_stripsAdapter.GetCount();
	}

	void Init()
	{
		LOG_FUNC_LN();
		ConfigureEffect();
	}

	void Update()
	{
		if (m_currentEffect)
		{
			m_currentEffect->Update();
		}
		else
		{
			EVERY_N_MILLIS(EFFECTS_PERIOD_DEFAULT)
			{
				FastLED.showColor(CRGB::Black);
			}
		}
	}

	template<typename T = IEffect>
	T * GetCurrentEffect() 
	{ 
		return dynamic_cast<T*>(m_currentEffect); 
	}

	void OnEffectSettingsChanged()
	{
		ConfigureEffect();
	}

	void SetStrip(int index, const Strip & strip)
	{
		if (strip.leds.start >= m_leds.start && strip.leds.start <= m_leds.end
		&& strip.leds.end >= m_leds.start && strip.leds.end <= m_leds.end)
		{
			m_stripsAdapter.SetStrip(index, strip);
		}
		else
		{
			LOG_LN("Failed to set strip due to strip leds range does not belong EffectsManager range");
		}
	}

private:
	EffectsManager() 
	{
		LOG_FUNC_LN();
	};
	~EffectsManager() { delete m_currentEffect; }
	void ConfigureEffect();

	LedsRange			m_leds;
	const EffectsData * m_effectsData;
	uint8_t				m_currentEffectNumber = 0;
	IEffect *			m_currentEffect = nullptr;
	LedStripsAdapter	m_stripsAdapter;
};