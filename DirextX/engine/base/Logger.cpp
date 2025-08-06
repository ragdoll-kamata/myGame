#include "Logger.h"
#include <Windows.h>
#include <debugapi.h>


void Logger::Log(const std::string& meessage)
{
	OutputDebugStringA(meessage.c_str());
}
