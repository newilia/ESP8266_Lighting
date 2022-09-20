#pragma once
#include "Singleton.h"
#include "GlobalDefs.h"
#include <Arduino.h>
#include <eeprom.h>
#include "Utils.h"

template <typename TData>
class SaveManagerTemplate : public Singleton<SaveManagerTemplate<TData>>
{
	TData			m_data;
	time_t			m_modifyTime = 0;
	uint16			m_autoSaveDelayMs = 1000;

public:
	SaveManagerTemplate()
	{
		EEPROM.begin(sizeof(TData));
		Load();
	};

	~SaveManagerTemplate()
	{
		EEPROM.end();
	}

	void Load()
	{
		EEPROM.get(0, m_data);
		if (TData::CURRENT_VERSION == m_data.version)
		{
			LOG("Data loaded, version =");
			LOG_LN(m_data.version);
		}
		else
		{
			LOG("Current settings version =");
			LOG(TData::CURRENT_VERSION);
			LOG(", but stored =");
			LOG_LN(m_data.version);
			ResetAndSave();
		}
	}

	void Save()
	{
		EEPROM.put(0, m_data);
		EEPROM.commit();
		m_modifyTime = 0;
		LOG_LN("Data saved");
	}

	const TData & GetData() const
	{
		return m_data;
	}

	TData & GetDataToChange()
	{
		m_modifyTime = millis();
		return m_data;
	}

	void Update()
	{
		if (m_modifyTime && m_autoSaveDelayMs && (millis() - m_modifyTime) >= m_autoSaveDelayMs)
		{
			Save();
		}
	}

	void ResetAndSave()
	{
		m_data = TData();
		Save();
	}
};