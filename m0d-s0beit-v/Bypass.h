#pragma once
#include "stdafx.h"
#include <psapi.h>

void BypassOnlineModelRequestBlock()
{
	DWORD64 dwGetModelTableFunctionAddress = Pattern::Scan(g_MainModuleInfo, "48 8B C8 FF 52 30 84 C0 74 05 48");
	if (dwGetModelTableFunctionAddress != NULL)
	{
		*reinterpret_cast<unsigned short*>(dwGetModelTableFunctionAddress + 0x8) = 0x9090;
	}
	else
	{
#ifndef __DEBUG
		Log::Error("Failed to find model table signature");
#else
		Log::Debug("Failed to find model table signature");
#endif
	}
}