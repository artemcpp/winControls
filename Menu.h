#pragma once

#include <windows.h>

namespace WinGui{
	#define MT_POPUP 1
	#define MAX_MENU_ITEMS 16
	class Menu;
};

class WinGui::Menu{
public:
	Menu(unsigned int _type = MT_POPUP);
	~Menu();

	HMENU CreateMenu(HMENU *save_to);
	unsigned int addMenuItem(char* name, unsigned int wID = NULL);
	bool show(int x, int y);
	void linkWithRightButton(bool yn);
	bool linkWithRightButton();

	void setHWND(HWND _hWnd);

private:
	unsigned int type;
	unsigned int *mID;

	MENUITEMINFO *mii;
	HMENU hMenu;
	HWND hWnd;
	int nItems;
	int nHMenu;
	bool RBLinked;
	bool err;
};
ImageBox
