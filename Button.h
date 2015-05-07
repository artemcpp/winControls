#pragma once

#include "Window.h"

namespace WinGui{
	class Button;
};

class WinGui::Button : public WinGui::Window{
public:
	Button();
	int text(LPCSTR _text);
	void setTextColor(COLORREF clr);

private:
	int write();
	int RefreshGraph();

	LPCSTR outText;
	RECT text_rect;
	COLORREF text_color;

};
