#include "stdafx.h"
#include "keyboard.h"
#include "inc\main.h"
#include "script.h"

MODULEINFO g_MainModuleInfo = { 0 };
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call)

{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hModule, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &g_MainModuleInfo, sizeof(g_MainModuleInfo)))
		{
			Log::Fatal("Unable to get MODULEINFO from GTA5.exe");
		}
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hModule);
		keyboardHandlerUnregister(OnKeyboardMessage);
		break;
	}
	return TRUE;
}
