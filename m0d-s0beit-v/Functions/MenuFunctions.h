#pragma once

char* strchr(std::string string);
char* ncchr(const char* constchar);

Hash $(char* hash);
Hash $(std::string hash);

float DegToRad(float degs);

typedef struct
{
	Hash modelHash;
	std::function<void()> spawnFunction;
} EntitySpawn;

void AddEntitySpawn(Hash hash, std::function<void()> spawnFunction);
void UpdateEntitySpawns();

typedef struct
{
	int initialDelay;
	int repeatSpeed;
	double totalDown;
	double sinceLastKeyPress;
	bool isPressed;
} Key;

void GetInput(int length, std::function<void(char*)> func);
void UpdateKeyPresses();
bool IsKeyPressed(BYTE key);
bool AreStringsEqual(char* string1, char* string2);
void ReleaseWatchedKeys();
bool MenuPairSort(std::pair<std::string, Menu > menu1, std::pair<std::string, Menu > menu2);

template <typename T> T modulo(T m, T n)
{
	return m >= 0 ? m % n : (n - abs(m % n)) % n;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);