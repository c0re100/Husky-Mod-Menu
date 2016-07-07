#pragma once

enum ROWTYPE
{
	SUBMENU,
	DYNAMICSUBMENU,
	ENTER,
	TOGGLE,
	TOGGLEF,
	INTSLIDER,
	INTSLIDERF,
	GETINPUT,
	BLANK
};

typedef struct
{
	ROWTYPE type;
	std::string name;
	bool* boolptr;
	int* intptr;
	int min;
	int max;
	int increment;
	char* nextmenu;
	bool autoEnter;
	std::function<void()> enterFunction;
} MenuRow;

typedef struct
{
	char* title;
	std::string previous;
	std::vector<MenuRow> rows;
} Menu;

MenuRow menurow(std::string name, char* nextmenu);
MenuRow enterrow(std::string name, std::function<void()> enterFunction);
MenuRow boolrow(std::string name, bool* boolptr);
MenuRow boolfuncrow(std::string name, bool* boolptr, std::function<void()> enterFunction);
MenuRow introw(std::string name, int* intptr, int min, int max, int increment);
MenuRow intfuncrow(std::string name, int* intptr, int min, int max, int increment, bool autoEnter, std::function<void()> enterFunction);
MenuRow blankrow(std::string name);
MenuRow dynmenurow(std::string name, char* nextmenu, std::function<void()> dynamicFunction);
MenuRow inputrow(std::string name, int length, std::function<void(char*)> func);
bool MenuRowSort(MenuRow row1, MenuRow row2);
