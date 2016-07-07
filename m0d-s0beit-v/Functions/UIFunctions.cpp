#include "../stdafx.h"

void DrawRect(float left, float top, float width, float height, color_t color)
{
	GRAPHICS::DRAW_RECT(left + width * 0.5f, top + height * 0.5f, width, height, color.red, color.green, color.blue, color.alpha);
}

void DrawSprite(char* dict, char* name, float left, float top, float width, float height, float rotation, color_t color)
{
	GRAPHICS::DRAW_SPRITE(dict, name, left + width * 0.5f, top + height * 0.5f, width, height, 0.0f, color.red, color.blue, color.green, color.alpha);
}

void DrawMenuLine(char* caption, float x, float y, float width, float height, int font, float scale, float margin, bool drawRect, Alignment alignment, color_t textColor, color_t rectColor)
{
	std::string menuTitle = std::string(caption);
	std::vector<std::string> splitTitle = split(menuTitle, '@');

	bool drawTitle = true;
	bool drawSprite = false;
	size_t size = splitTitle.size();

	if (size == 2)
	{
		drawTitle = false;
		drawSprite = true;
	}
	else if (size == 3)
	{
		caption = strchr(splitTitle[2]);
		drawSprite = true;
		if (AreStringsEqual(caption, "SPRITE"))
		{
			drawTitle = false;
			rectColor.red = 255;
			rectColor.green = 255;
			rectColor.blue = 255;
			rectColor.alpha = 255;
		}
	}
	else if (size == 5)
	{
		drawTitle = false;
		drawSprite = true;
		rectColor.red = atoi(strchr(splitTitle[2]));
		rectColor.green = atoi(strchr(splitTitle[3]));
		rectColor.blue = atoi(strchr(splitTitle[4]));
		rectColor.alpha = 255;
	}
	else if (size == 6)
	{
		drawTitle = false;
		drawSprite = true;
		rectColor.red = atoi(strchr(splitTitle[2]));
		rectColor.green = atoi(strchr(splitTitle[3]));
		rectColor.blue = atoi(strchr(splitTitle[4]));
		rectColor.alpha = atoi(strchr(splitTitle[5]));
	}

	float textWidth = GetTextWidth(caption, font, scale);
	float fontY = y + GetCenterHeightOffset(font, scale, height);

	if (drawTitle)
	{
		switch (alignment)
		{
		case Left:
			DrawTextUI(caption, x + margin, fontY, font, scale, textColor);
			break;
		case Right:
			DrawTextUI(caption, x + width - margin - textWidth, fontY, font, scale, textColor);
			break;
		case Center:
			DrawTextUI(caption, x + GetCenterOffset(textWidth, width), fontY, font, scale, textColor);
			break;
		}		
	}

	if (drawRect)
	{		
		if (drawSprite)
		{
			char* spriteDict = strchr(splitTitle[0]);
			char* spriteName = strchr(splitTitle[1]);

			if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(spriteDict))
			{
				GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(spriteDict, TRUE);
			}

			DrawSprite(spriteDict, spriteName, x, y, width, height, 0.0f, rectColor);
		}
		else
		{
			DrawRect(x, y, width, height, rectColor);
		}
	}
}

void DrawTextUI(char* text, float x, float y, int font, float scale, color_t color)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, scale);
	UI::SET_TEXT_COLOUR(color.red, color.green, color.blue, color.alpha);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	UI::_DRAW_TEXT(x, y);
}

void DrawMsgUI(char* text, float x, float y, float scale, color_t color)
{
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(0.4f, scale);
	UI::SET_TEXT_COLOUR(color.red, color.green, color.blue, color.alpha);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	UI::_DRAW_TEXT(x, y);
}

void DrawLine(float x, float y, float width, float height, color_t color)
{
	DrawRect(x, y, width, height, color);
}

void DrawNotification(std::string str)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&str[0u]);
	UI::_SET_NOTIFICATION_MESSAGE("CHAR_DEFAULT", "CHAR_DEFAULT", TRUE, 1, XorStr("Husky Mod Menu"), XorStr("Modding By Husky"));
}

void DrawChat(std::string str)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&str[0u]);
	UI::_SET_NOTIFICATION_MESSAGE("CHAR_DEFAULT", "CHAR_DEFAULT", TRUE, 1, XorStr("Husky Mod Menu"), XorStr("Private Chatroom"));
}

float GetCenterOffset(float textWidth, float rectWidth)
{
	return (rectWidth - textWidth) / 2;
}

float GetTextWidth(char* text, int font, float scale)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, scale);
	UI::_SET_TEXT_ENTRY_FOR_WIDTH("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	return UI::_GET_TEXT_SCREEN_WIDTH(TRUE);
}

float GetCenterHeightOffset(int font, float scale, float height)
{
	switch (font)
	{
	case 0:
		return (height - 0.05f * scale) / 2 - 0.01f * scale;
	case 1:
		return (height - 0.04f * scale) / 2 - 0.007f * scale;	
	case 2:
		return (height - 0.045f * scale) / 2 - 0.009f * scale;
	case 4:
		return (height - 0.045f * scale) / 2 - 0.009f * scale;
	case 6:
		return (height - 0.045f * scale) / 2 - 0.009f * scale;
	case 7:
		return (height - 0.04f * scale) / 2 - 0.008f * scale;
	default:
		return (height - 0.045f * scale) / 2 - 0.008f * scale;
	}
}