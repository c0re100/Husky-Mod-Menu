#include "stdafx.h"

MenuRow menurow(std::string name, char* nextmenu)
{
	return {SUBMENU, name, nullptr, nullptr, NULL, NULL, NULL, nextmenu, false, nullptr};
}

MenuRow enterrow(std::string name, std::function<void()> enterFunction)
{
	return {ENTER, name, nullptr, nullptr, NULL, NULL, NULL, nullptr,false, enterFunction};
}

MenuRow boolrow(std::string name, bool* boolptr)
{
	return {TOGGLE, name, boolptr, nullptr, NULL, NULL, NULL, nullptr,false, nullptr};
}

MenuRow boolfuncrow(std::string name, bool* boolptr, std::function<void()> enterFunction)
{
	return {TOGGLEF, name, boolptr, nullptr, NULL, NULL, NULL, nullptr,false, enterFunction};
}

MenuRow introw(std::string name, int* intptr, int min, int max, int increment)
{
	return {INTSLIDER, name, nullptr, intptr, min, max, increment, nullptr, false,nullptr};
}

MenuRow intfuncrow(std::string name, int* intptr, int min, int max, int increment, bool autoEnter, std::function<void()> enterFunction)
{
	return {INTSLIDERF, name, nullptr, intptr, min, max, increment, nullptr, autoEnter, enterFunction};
}

MenuRow blankrow(std::string name)
{
	return {BLANK, name, nullptr, nullptr, NULL, NULL, NULL, nullptr, false,nullptr};
}

MenuRow dynmenurow(std::string name, char* nextmenu, std::function<void()> dynamicFunction)
{
	return {DYNAMICSUBMENU, name, nullptr, nullptr, NULL, NULL, NULL, nextmenu, false,dynamicFunction};
}

MenuRow inputrow(std::string name, int length, std::function<void(char*)> func)
{
	return {GETINPUT, name, nullptr, nullptr, NULL, NULL, NULL, nullptr, false, ([=] { GetInput(length, func); })};
}

bool MenuRowSort(MenuRow row1, MenuRow row2)
{
	return row1.name < row2.name;
}

