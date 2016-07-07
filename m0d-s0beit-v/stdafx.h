// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN

#define _USE_MATH_DEFINES // For M_PI

// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include <string>
#include <vector>
#include <sstream>
#include <Psapi.h>
#include <iostream>
#include <fstream>
#include <timeapi.h>
#include <functional>
#include <map>
#include <queue>
#include <deque>
#include <time.h>
#include <string.h>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <stdint.h>
#include <cassert>
#include <cmath>
#include <cstddef>  // for std::ptrdiff_t
#include <cstdio>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>
#include <regex>
#include <MMSystem.h>
#include <initializer_list>
#include <cassert>
#include <math.h>
#include <thread>
#include <process.h>
#include <mutex>
#include <condition_variable>
#include <mutex>
#include <tchar.h>
#include <memory>


#pragma comment(lib, "winmm.lib")

extern MODULEINFO g_MainModuleInfo;

// Mine
#include "Log.h"
#include "Pattern.h"
#include "XorString.h"
#include "format.h"

// Rockstar
#include "inc\natives.h"
#include "inc\types.h"
#include "inc\main.h"
#include "Hashes.h"
#include "VehicleValues.h"


// Main
#include "script.h"
#include "menu.h"
#include "MenuRow.h"

#include "Functions/MenuFunctions.h"
#include "Functions/UIFunctions.h"
#include "Functions/PlayerFunctions.h"
#include "Functions/OnlineFunctions.h"
#include "Functions/WorldFunctions.h"
#include "Functions/WeaponFunctions.h"
#include "Functions/VehicleFunctions.h"

template <typename T> T random()
{
	T value;
	char* pointer = reinterpret_cast<char*>(&value);

	for (int i = 0; i < sizeof(T); ++i)
	{
		pointer[i] = char(rand() % 0xFF);
	}

	return value;
}

#define VK_0             0x30
#define VK_1             0x31
#define VK_2             0x32
#define VK_3             0x33
#define VK_4             0x34
#define VK_5             0x35
#define VK_6             0x36
#define VK_7             0x37
#define VK_8             0x38
#define VK_9             0x39

#define VK_A             0x41
#define VK_B             0x42
#define VK_C             0x43
#define VK_D             0x44
#define VK_E             0x45
#define VK_F             0x46
#define VK_G             0x47
#define VK_H             0x48
#define VK_I             0x49
#define VK_J             0x4A
#define VK_K             0x4B
#define VK_L             0x4C
#define VK_M             0x4D
#define VK_N             0x4E
#define VK_O             0x4F
#define VK_P             0x50
#define VK_Q             0x51
#define VK_R             0x52
#define VK_S             0x53
#define VK_T             0x54
#define VK_U             0x55
#define VK_V             0x56
#define VK_W             0x57
#define VK_X             0x58
#define VK_Y             0x59
#define VK_Z             0x5A
