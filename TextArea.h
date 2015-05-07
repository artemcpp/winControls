#pragma once

#include <windows.h>
#include "Window.h"

namespace WinGui{
	class TextBox;
};

#define TEXTAREA DT_WORDBREAK
#define TEXTLINE DT_SINGLELINE | DT_VCENTER

class WinGui::TextBox : public Window{
public:
	TextBox(UINT type = TEXTAREA);

	int text(LPCSTR text);
	void textColor(COLORREF clr);
	void textFormat(UINT _format);

	void AutoResize(bool YorN, int WMax = 0, int HMax = 0);

protected:
	int RefreshGraph();
	int write();

	LPCSTR outText;
	UINT text_size;
	int w_max, h_max;
	SIZE text_px_size;
	UINT format;
	TEXTMETRIC text_info;
	bool resize_wnd;

	RECT text_rect;
	COLORREF text_color;
};

