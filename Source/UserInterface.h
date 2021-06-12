#pragma once
extern enum SubMenus;

namespace Menu
{
	class UserInterface
	{
	public:
		bool reggedytd = false;
		bool opened = false;
		bool selectPressed = false;
		bool leftPressed = false;
		bool rightPressed = false;
		bool menusounds = true;

		bool Option(const char* option);
		bool Option(const char* option, std::function<void()> function);
		bool SubMenu(const char* option, SubMenus newSub);
		bool SubMenu(const char* option, SubMenus newSub, std::function<void()> function);
		bool Bool(const char* option, bool& b00l);
		bool Int(const char* option, int& _int, int min, int max);
		bool Int(const char* option, int& _int, int min, int max, int step);
		bool Float(const char* option, float& _float, int min, int max);
		bool Float(const char* option, float& _float, int min, int max, int step);
		bool IntVector(const char* option, std::vector<int> Vector, int& position);
		bool FloatVector(const char* option, std::vector<float> Vector, int& position);
		bool StringVector(const char* option, std::vector<std::string> Vector, int& position);
		bool StringVector(const char* option, std::vector<char*> Vector, int& position);
	public:
		int textureID;
		int maxVisOptions = 11;
		int currentOption = 1;
		int optionCount = 0;

		int menuLevel = 0;
		int optionsArray[1000];

		int keyPressDelay = 140;
		int keyPressPreviousTick = GetTickCount();
		int openKey = VK_F8;
		int backKey = VK_BACK;
		int upKey = VK_UP;
		int downKey = VK_DOWN;
		int leftKey = VK_LEFT;
		int rightKey = VK_RIGHT;
		int selectKey = VK_RETURN;
	public:
		SubMenus currentMenu;
		SubMenus menusArray[1000];
	public:
		float menuX = 0.85f;
		float menuWidth = 0.21f;
	public:
		RGBA titlerect = { 248, 225, 5, 255 };
		RGBA optionrect = { 0, 0, 0, 130 };
		RGBA scroller = { 248, 225, 5, 210 };
	public:
		RGBAF titletext = { 255, 255, 255, 255, 7 };
		RGBAF optiontext = { 255, 255, 255, 255, 6 };
	public:
		void Keys();
		void MoveMenu(SubMenus menu);
		void BackMenu();

		void Title(const char* title);
		void End();
	};
	inline UserInterface gui;

	class Drawing
	{
	public:
		void Text(const char* text, RGBAF rgbaf, VECTOR2 position, VECTOR2_2 size, bool center);
		void Rect(RGBA rgba, VECTOR2 position, VECTOR2_2 size);
		void Sprite(std::string Streamedtexture, std::string textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a);
	};
	inline Drawing draw;
}