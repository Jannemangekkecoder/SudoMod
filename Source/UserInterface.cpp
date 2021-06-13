#include "stdafx.h"

namespace Menu
{
	void Menu::Drawing::Text(const char* text, RGBAF rgbaf, VECTOR2 position, VECTOR2_2 size, bool center)
	{
		HUD::SET_TEXT_SCALE(size.w, size.h);
		HUD::SET_TEXT_FONT(rgbaf.f);
		HUD::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
		HUD::SET_TEXT_CENTRE(center);
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y, NULL);

	}
	void Menu::Drawing::Sprite(std::string Streamedtexture, std::string textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
	{
		if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)Streamedtexture.c_str()))
		{
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((char*)Streamedtexture.c_str(), false);
		}
		else
		{
			GRAPHICS::DRAW_SPRITE((char*)Streamedtexture.c_str(), (char*)textureName.c_str(), x, y, width, height, rotation, r, g, b, a, NULL);
		}
	}

	void Menu::Drawing::Rect(RGBA rgba, VECTOR2 position, VECTOR2_2 size)
	{
		GRAPHICS::DRAW_RECT(position.x, position.y, size.w, size.h, rgba.r, rgba.g, rgba.b, rgba.a, NULL);
	}

	void UserInterface::Title(const char* title)
	{
		if (gui.textureID != 0) // If no YTD found, it will draw text and a rect itself. 
		{
			draw.Sprite("SudoMod", "EpicHeader", menuX, 0.0750f, menuWidth, 0.1f, 0, 255, 255, 255, 255); /* https://www.unknowncheats.me/forum/3158178-post18.html */
		}
		else
		{
			draw.Text("SudoMod", titletext, { menuX, 0.055f }, { 0.85f, 0.85f }, true);
			draw.Rect(titlerect, { menuX, 0.0750f }, { menuWidth, 0.1f });
		}

		// SubmenuBar
		draw.Rect({ 0, 0, 0, 255 }, { menuX, (optionCount + 1) * 0.035f + 0.1065f }, { menuWidth, 0.035f });
		draw.Text(title, { 255, 255, 255, 255, 6 }, { menuX, (optionCount) * 0.035f + 0.127f }, { 0.4f, 0.4f }, true);

		// Disable Controls
		HUD::HIDE_HELP_TEXT_THIS_FRAME();
		CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(10);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(6);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(7);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(9);
		HUD::HIDE_HUD_COMPONENT_THIS_FRAME(8);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_NEXT_CAMERA, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_CHARACTER_WHEEL, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_LIGHT, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_HEAVY, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_MULTIPLAYER_INFO, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_ALTERNATE, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_VEH_CIN_CAM, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_MAP_POI, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_VEH_RADIO_WHEEL, true);
		PAD::DISABLE_CONTROL_ACTION(2, INPUT_VEH_HEADLIGHT, true);
	}

	bool UserInterface::Option(const char* option)
	{
		optionCount++;
		bool onThis = currentOption == optionCount ? true : false;
		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
		{
			draw.Rect(optionrect, { menuX, (optionCount) * 0.035f + 0.1415f }, { menuWidth, 0.035f });
			draw.Text(option, optiontext, { menuX - 0.1f, (optionCount) * 0.035f + 0.125f }, { 0.45f, 0.45f }, false);
			onThis ? draw.Rect(scroller, { menuX, (optionCount) * 0.035f + 0.1415f }, { menuWidth, 0.035f }) : NULL;
		}
		else if (optionCount > (currentOption - maxVisOptions) && optionCount <= currentOption)
		{
			draw.Rect(optionrect, { menuX,  (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.1415f }, { menuWidth, 0.035f });
			draw.Text(option, optiontext, { menuX - 0.1f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.125f }, { 0.45f, 0.45f }, false);
			onThis ? draw.Rect(scroller, { menuX,  (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.1415f }, { menuWidth, 0.035f }) : NULL;
		}
		if (currentOption == optionCount)
		{
			if (selectPressed)
			{
				return true;
			}
		}
		return false;
	}
	bool UserInterface::Option(const char* option, std::function<void()> function)
	{
		Option(option);

		if (optionCount == currentOption && selectPressed) {
			function();
			return true;
		}
		return false;
	}

	bool UserInterface::SubMenu(const char* option, SubMenus newSub)
	{
		Option(option);

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(">>", optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(">>", optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) {
			MoveMenu(newSub);
			return true;
		}
		return false;
	}

	bool UserInterface::SubMenu(const char* option, SubMenus newSub, std::function<void()> function)
	{
		SubMenu(option, newSub);

		if (optionCount == currentOption && selectPressed) {
			function();
			return true;
		}
		return false;
	}

	bool UserInterface::Bool(const char* option, bool& b00l)
	{
		Option(option);

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(b00l ? "~g~ON" : "~r~OFF", optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(b00l ? "~g~ON" : "~r~OFF", optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) {
			b00l ^= 1;
			return true;
		}
		return false;
	}

	bool UserInterface::Int(const char* option, int& _int, int min, int max)
	{
		Option(option);

		if (optionCount == currentOption) {
			if (leftPressed) {
				_int >= min ? _int-- : _int = max;
			}
			if (rightPressed) {
				_int < max ? _int++ : _int = min;
			}
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(StringToChar(std::to_string(_int)), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(StringToChar(std::to_string(_int)), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}

	bool UserInterface::Int(const char* option, int& _int, int min, int max, int step)
	{
		Option(option);

		if (optionCount == currentOption) {
			if (leftPressed) {
				_int >= min ? _int -= step : _int = max;
			}
			if (rightPressed) {
				_int < max ? _int += step : _int = min;
			}
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(StringToChar(std::to_string(_int)), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(StringToChar(std::to_string(_int)), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}

#pragma warning(disable: 4244)
	bool UserInterface::Float(const char* option, float& _float, int min, int max)
	{
		Option(option);

		if (optionCount == currentOption) {
			if (leftPressed) {
				_float <= min ? _float = max : _float -= 0.1f;
			}
			if (rightPressed) {
				_float >= max ? _float = min : _float += 0.1f;
			}
			_float < min ? _float = max : _float > max ? _float = min : NULL;
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(StringToChar(std::to_string(_float)), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(StringToChar(std::to_string(_float)), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}

	bool UserInterface::Float(const char* option, float& _float, int min, int max, int step)
	{
		Option(option);

		if (optionCount == currentOption) {
			if (leftPressed) {
				_float <= min ? _float = max : _float -= 0.1f;
			}
			if (rightPressed) {
				_float >= max ? _float = min : _float += 0.1f;
			}
			_float < min ? _float = max : _float > max ? _float = min : NULL;
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(StringToChar(std::to_string(_float)), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(StringToChar(std::to_string(_float)), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}
#pragma warning(default: 4244)

#pragma warning(disable: 4267)
	bool UserInterface::IntVector(const char* option, std::vector<int> Vector, int& position)
	{
		Option(option);

		if (optionCount == currentOption) {
			int max = Vector.size() - 1;
			int min = 0;
			if (leftPressed) {
				position >= 1 ? position-- : position = max;
			}
			if (rightPressed) {
				position < max ? position++ : position = min;
			}
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(StringToChar(std::to_string(Vector[position])), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(StringToChar(std::to_string(Vector[position])), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}

	bool UserInterface::FloatVector(const char* option, std::vector<float> Vector, int& position)
	{
		Option(option);

		if (optionCount == currentOption) {
			size_t max = Vector.size() - 1;
			int min = 0;
			if (leftPressed) {
				position >= 1 ? position-- : position = max;
			}
			if (rightPressed) {
				position < max ? position++ : position = min;
			}
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(StringToChar(std::to_string(Vector[position])), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(StringToChar(std::to_string(Vector[position])), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}

	bool UserInterface::StringVector(const char* option, std::vector<std::string> Vector, int& position)
	{
		Option(option);

		if (optionCount == currentOption) {
			size_t max = Vector.size() - 1;
			int min = 0;
			if (leftPressed) {
				position >= 1 ? position-- : position = max;
			}
			if (rightPressed) {
				position < max ? position++ : position = min;
			}
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(StringToChar((Vector[position])), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(StringToChar((Vector[position])), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}

	bool UserInterface::StringVector(const char* option, std::vector<char*> Vector, int& position)
	{
		Option(option);

		if (optionCount == currentOption) {
			size_t max = Vector.size() - 1;
			int min = 0;
			if (leftPressed) {
				position >= 1 ? position-- : position = max;
			}
			if (rightPressed) {
				position < max ? position++ : position = min;
			}
		}

		if (currentOption <= maxVisOptions && optionCount <= maxVisOptions)
			draw.Text(((Vector[position])), optiontext, { menuX + 0.068f, optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
		else if (optionCount > currentOption - maxVisOptions && optionCount <= currentOption)
			draw.Text(((Vector[position])), optiontext, { menuX + 0.068f, (optionCount - (currentOption - maxVisOptions)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

		if (optionCount == currentOption && selectPressed) return true;
		else if (optionCount == currentOption && leftPressed) return true;
		else if (optionCount == currentOption && rightPressed) return true;
		return false;
	}
#pragma warning(default: 4267)

	void UserInterface::End()
	{
		if (optionCount >= maxVisOptions) {
			draw.Rect({ 0, 0, 0, 210 }, { menuX, (maxVisOptions + 1) * 0.035f + 0.1415f }, { menuWidth, 0.035f });
			draw.Text(StringToChar(std::to_string(currentOption) + "/" + std::to_string(optionCount)), { 255, 255, 255, 255, 6 }, { menuX - 0.1f, (maxVisOptions + 1) * 0.035f + 0.125f }, { 0.5f, 0.5f }, false);
			draw.Text("1.0", { 255, 255, 255, 255, 6 }, { menuX + 0.088f, (maxVisOptions + 1) * 0.035f + 0.125f }, { 0.5f, 0.5f }, false);

			/*Switches the Arrow on optioncount*/
			if (currentOption == 1) {
				draw.Sprite("commonmenu", "arrowright", menuX, (maxVisOptions + 1) * 0.035f + 0.142f, 0.010f, 0.0175f, 90, 255, 255, 255, 255);
			}
			else if (currentOption == optionCount) {
				draw.Sprite("commonmenu", "arrowright", menuX, (maxVisOptions + 1) * 0.035f + 0.142f, 0.010f, 0.0175f, 270, 255, 255, 255, 255);
			}
			else {
				draw.Sprite("commonmenu", "shop_arrows_upanddown", menuX, (maxVisOptions + 1) * 0.035f + 0.140f, 0.020f, 0.035f, 180, 255, 255, 255, 255);
			}
		}
		else if (optionCount > 0) {
			draw.Rect({ 0, 0, 0, 210 }, { menuX, (optionCount + 1) * 0.035f + 0.1415f }, { menuWidth, 0.035f });
			draw.Text(StringToChar(std::to_string(currentOption) + "/" + std::to_string(optionCount)), { 255, 255, 255, 255, 6 }, { menuX - 0.1f, (optionCount + 1) * 0.035f + 0.125f }, { 0.5f, 0.5f }, false);
			draw.Text("1.0", { 255, 255, 255, 255, 6 }, { menuX + 0.088f, (optionCount + 1) * 0.035f + 0.125f }, { 0.5f, 0.5f }, false);

			/*Switches the Arrow on optioncount*/
			if (currentOption == 1 && optionCount > 1) {
				draw.Sprite("commonmenu", "arrowright", menuX, ((optionCount + 1) * 0.035f + 0.142f), 0.010f, 0.0175f, 90, 255, 255, 255, 255);
			}
			else if (currentOption == optionCount && optionCount > 1) {
				draw.Sprite("commonmenu", "arrowright", menuX, ((optionCount + 1) * 0.035f + 0.142f), 0.010f, 0.0175f, 270, 255, 255, 255, 255);
			}
			else {
				draw.Sprite("commonmenu", "shop_arrows_upanddown", menuX, ((optionCount + 1) * 0.035f + 0.140f), 0.020f, 0.035f, 180, 255, 255, 255, 255);
			}
		}
	}

#pragma warning(disable: 4018)
	void UserInterface::Keys()
	{
		selectPressed = false;
		leftPressed = false;
		rightPressed = false;
		if (GetTickCount() - keyPressPreviousTick > keyPressDelay) {
			if (GetAsyncKeyState(openKey)) {
				menuLevel == 0 ? MoveMenu(SubMenus::MAINMENU) : menuLevel == 1 ? BackMenu() : NULL;

				if (opened) { MoveMenu(SubMenus::NOMENU); opened = false; } else if (!opened) { MoveMenu(SubMenus::MAINMENU); opened = true; } if (menusounds) { if (opened) { AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); } else { AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); } }
				
				keyPressPreviousTick = GetTickCount();
			}
			else if (GetAsyncKeyState(backKey)) {
				menuLevel > 0 ? BackMenu() : NULL;

				if (menusounds) { AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); }

				keyPressPreviousTick = GetTickCount();
			}
			else if (GetAsyncKeyState(upKey)) {
				currentOption > 1 ? currentOption-- : currentOption = optionCount;

				if (menusounds) { AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); }

				keyPressPreviousTick = GetTickCount();
			}
			else if (GetAsyncKeyState(downKey)) {
				currentOption < optionCount ? currentOption++ : currentOption = 1;

				if (menusounds) { AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); }

				keyPressPreviousTick = GetTickCount();
			}
			else if (GetAsyncKeyState(leftKey)) {
				leftPressed = true;

				if (menusounds) { AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); }

				keyPressPreviousTick = GetTickCount();
			}
			else if (GetAsyncKeyState(rightKey)) {
				rightPressed = true;

				if (menusounds) { AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); }

				keyPressPreviousTick = GetTickCount();
			}
			else if (GetAsyncKeyState(selectKey)) {
				selectPressed = true;

				if (menusounds) { AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false); }

				keyPressPreviousTick = GetTickCount();
			}
		}
		optionCount = 0;
	}
#pragma warning(default : 4018)

	void UserInterface::MoveMenu(SubMenus menu)
	{
		menusArray[menuLevel] = currentMenu;
		optionsArray[menuLevel] = currentOption;
		menuLevel++;
		currentMenu = menu;
		currentOption = 1;
	}

	void UserInterface::BackMenu()
	{
		menuLevel--;
		currentMenu = menusArray[menuLevel];
		currentOption = optionsArray[menuLevel];
	}
}