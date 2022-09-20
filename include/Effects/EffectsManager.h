#pragma once
#include <FastLED.h>
#include "Effects/EffectsBase.h"
#include "Singleton.h"
#include "ShevesLedsAdapter.h"
#include "SaveData.h"

class EffectsManager : public Singleton<EffectsManager>
{
	friend class Singleton<EffectsManager>;
public:
	void Init(Leds leds, const SaveData * saveData);
	void Update();
	ShelvesLedsAdapter & GetShelvesAdapter() { return m_shelvesAdapter; }

	template<typename T = IEffect>
	T * GetCurrentEffect() { return dynamic_cast<T*>(m_currentEffect); }

	void OnEffectSettingsChanged();

private:
	EffectsManager() {};
	~EffectsManager() { delete m_currentEffect; }
	void ConfigureEffect();

	Leds				m_leds;
	const SaveData * 	m_saveData;
	uint8_t				m_currentEffectNumber = 0;
	IEffect *			m_currentEffect = nullptr;
	ShelvesLedsAdapter	m_shelvesAdapter;
};