#pragma once
#include "stdafx.h"
#include "keyboard.h"
//#include "menu.h"
#include "Bypass.h"

DWORD64 TunablesPointer()
{
	return *reinterpret_cast<DWORD64*>(DWORD64(g_MainModuleInfo.lpBaseOfDll) + *reinterpret_cast<int*>(Pattern::Scan(g_MainModuleInfo, "48 8B 8C C2 ? ? ? ? 48 85 C9 74 19") + 4) + 8);
}

void process_main_menu()
{
	UpdateMenu();
}

void update_features()
{
	
}

void main()
{
	//reset_globals();

	while (true)
	{
		process_main_menu();
		//update_features();
		WAIT(0);
	}
}

void ScriptMain()
{
	BypassOnlineModelRequestBlock();
	srand(GetTickCount());
	main();
}