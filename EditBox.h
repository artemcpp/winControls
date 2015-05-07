#pragma once

#include <windows.h>
#include <richedit.h>
#include "Window.h"

namespace WinGui{
	class EditBox;
};

class WinGui::EditBox : public WinGui::Window{
public:
	EditBox(unsigned int _type = NULL);
	~EditBox();
	void SelectUrl(bool yn = false);
	void setBgColor(COLORREF clr);
	void setTextColor(COLORREF txt_clr);

	int getText(char* to);
	int setText(char* str);
	int length();
private:
	char* text;
	CHARFORMAT2 format;
	unsigned int type;

};
