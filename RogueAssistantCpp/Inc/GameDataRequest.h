#pragma once
#include "Defines.h"
#include <functional>
#include <vector>

struct GameDataRequest
{
	enum RequestType
	{
		REQUEST_READ,
		REQUEST_WRITE,
	};

	RequestType m_Type;
	size_t m_Address;
	size_t m_Size;
	std::vector<u8> m_Data;
	std::function<void(std::vector<u8> const& data)> m_Callback;
};