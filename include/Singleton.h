#pragma once

template<typename T>
class Singleton
{
public:
	static T & instance()
	{
		if (!sm_instance)
		{
			sm_instance = new T;
		}
		return *sm_instance;
	}
private:
	inline static T * sm_instance = nullptr;
};