#pragma once
#include "../stdafx.h"

enum Alignment
{
	Left,
	Right,
	Center
};

void DrawRect(float left, float top, float width, float height, color_t color);
void DrawSprite(char* dict, char* name, float left, float top, float width, float height, float rotation, color_t color);
void DrawMenuLine(char* caption, float x, float y, float width, float height, int font, float scale, float margin, bool drawRect, Alignment alignment, color_t textColor, color_t rectColor);
void DrawTextUI(char* text, float x, float y, int font, float scale, color_t color);
void DrawMsgUI(char* text, float x, float y, float scale, color_t color);
void DrawNotification(std::string str);
void DrawChat(std::string str);
float GetCenterOffset(float textWidth, float rectWidth);
float GetTextWidth(char* text, int font, float scale);
void DrawLine(float x, float y, float width, float height, color_t color);
float GetCenterHeightOffset(int font, float scale, float height);

	