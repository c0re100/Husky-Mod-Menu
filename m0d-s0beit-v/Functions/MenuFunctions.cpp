#include "../stdafx.h"

char* strchr(std::string string)
{
	return ncchr(string.c_str());
}

char* ncchr(const char* constchar)
{
	return _strdup(constchar);
}

Hash $(char* hash)
{
	return GAMEPLAY::GET_HASH_KEY(hash);
}

Hash $(std::string hash)
{
	return GAMEPLAY::GET_HASH_KEY(strchr(hash));
}

float DegToRad(float degs)
{
	return float(degs * M_PI / 180.0f);
}

static std::queue<EntitySpawn> WaitingToSpawn;

void AddEntitySpawn(Hash hash, std::function<void()> spawnFunction)
{
	WaitingToSpawn.push({ hash, spawnFunction });
	STREAMING::REQUEST_MODEL(hash);
}

void UpdateEntitySpawns()
{
	if (!WaitingToSpawn.empty())
	{
		EntitySpawn currSpawn = WaitingToSpawn.front();

		if (STREAMING::HAS_MODEL_LOADED(currSpawn.modelHash))
		{
			currSpawn.spawnFunction();
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(currSpawn.modelHash);
			WaitingToSpawn.pop();
		}
	}
}

void GetInput(int length, std::function<void(char*)> func)
{
	ReleaseWatchedKeys();

	WAIT(0);

	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", length == -1 ? -1 : length + 1);

	/*
		0 - User still editing
		1 - User has finished editing
		2 - User has canceled editing
		3 - Keyboard isn't active
	*/

	while (true)
	{
		int state = GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD();
		if (state == 1)
		{
			char* result = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
			if (strlen(result) > 0)
			{
				func(_strdup(result));
			}
			return;
		}
		if (state != 0)
		{
			return;
		}

		WAIT(0);
	}
}

/*
	0x0000 - Not pressed, not toggled
	0x8000 - Pressed, not toggled
	0x0001 - Not pressed, toggled
	0x8001 - Pressed, toggled
*/

static std::map<BYTE, Key> Keys = // Timings for key repeats
{
	{ VK_NUMPAD4, { 300, 300			} }, // Key ID, { Initial Delay (Miliseconds), Repeat Speed (Miliseconds) }
	{ VK_NUMPAD6, { 300, 300			} },
	{ VK_NUMPAD8, { 100, 300		} },
	{ VK_NUMPAD2, { 100, 300		} },
	{ VK_NUMPAD5, { 100, 300		} }, 
	{ VK_F5,	  { 999999, 999999	} }, // We dont want it spamming
	{ VK_NUMPAD0, { 999999, 999999	} },
	{ VK_DECIMAL, { 999999, 999999	} },
};

static clock_t lastUpdate;

void UpdateKeyPresses()
{
	clock_t currTime = clock();

	double elapsed = max(1, double(currTime - lastUpdate));	// Make sure it's never 0, else you would get key pressed twice

	lastUpdate = currTime;

	for each (auto keyPair in Keys)
	{
		keyPair.second.isPressed = false;

		if (GetAsyncKeyState(keyPair.first) && 0x8000)
		{
			if (keyPair.second.totalDown == 0)
			{
				keyPair.second.isPressed = true;
			}
			else if (keyPair.second.totalDown > keyPair.second.initialDelay)
			{
				if (keyPair.second.sinceLastKeyPress == 0)
				{
					keyPair.second.isPressed = true;
				}
				else if (keyPair.second.sinceLastKeyPress > keyPair.second.repeatSpeed)
				{
					keyPair.second.sinceLastKeyPress -= keyPair.second.repeatSpeed;
					keyPair.second.isPressed = true;
				}
				keyPair.second.sinceLastKeyPress += elapsed;
			}

			keyPair.second.totalDown += elapsed;
		}
		else
		{
			keyPair.second.totalDown = 0;
			keyPair.second.sinceLastKeyPress = 0;
			keyPair.second.isPressed = false;
		}

		Keys[keyPair.first] = keyPair.second; // The for each only gives a 'temp' 
	}
}

void ReleaseWatchedKeys()
{
	for each(std::pair<BYTE, Key> key in Keys)
	{
		keybd_event(key.first, 0, KEYEVENTF_KEYUP, 0);
	}
}

bool IsKeyPressed(BYTE key)
{
	return Keys[key].isPressed;
}

bool AreStringsEqual(char* string1, char* string2)
{
	return strcmp(string1, string2) == 0;
	
}

bool MenuPairSort(std::pair<std::string, Menu > menu1, std::pair<std::string, Menu > menu2)
{
	return menu1.second.title < menu2.second.title;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void GetModelDimensions(Hash modelHash, float& width, float& length, float& height)
{
	Vector3 min, max;
	GAMEPLAY::GET_MODEL_DIMENSIONS(modelHash, &min, &max);
	width = max.y - min.y;
	length = max.x - min.x;
	height = max.z - min.z;
}