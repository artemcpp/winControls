#include "TextArea.h"

WinGui::TextBox::TextBox(UINT type){
	text_color = RGB(0, 0, 0);
	format = type;
	resize_wnd = false;

	setClassName("textbox123");
	setType(CS_HREDRAW | CS_VREDRAW);
	TitleLine(false);
}

int WinGui::TextBox::text(LPCSTR text){
	if(text == NULL) return -1;
	int diff, extra, n_str;
	diff = extra = 0;
	n_str = 1;
	outText = text;
	text_size = strlen(text);


	if(resize_wnd == true){
		hDC = GetDC(hWnd);
		GetTextExtentPoint32(hDC, outText, text_size, &text_px_size);
		GetTextMetrics(hDC, &text_info);

		if(w_max != 0)
			if(text_px_size.cx > w_max){
					diff = text_px_size.cx;
					extra = diff % w_max;
					diff /= w_max;
					n_str += diff;
					text_px_size.cy = text_info.tmHeight * n_str;
					
					text_px_size.cx = w_max;
			}
		if(h_max != 0)
			if(text_px_size.cy > h_max)
				text_px_size.cy = h_max;

		size(text_px_size.cx, text_px_size.cy);
	}

	write();
	
	//RefreshGraph();

	return 0;
}

int WinGui::TextBox::write(){
	if(outText == NULL || *outText == NULL) return -1;
	GetClientRect(hWnd, &text_rect);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, text_color);
	DrawText(hDC, outText, text_size, &text_rect, format);

	return 0;
}

void WinGui::TextBox::textColor(COLORREF clr){
	text_color = clr;
}

void WinGui::TextBox::textFormat(UINT _format){
	format |= _format;
}

void WinGui::TextBox::AutoResize(bool YorN, int WMax, int HMax){
	w_max = WMax;
	h_max = HMax;

	resize_wnd = YorN;
}

int WinGui::TextBox::RefreshGraph(){
	if(hbtmp){
	bits = CreateCompatibleDC(hDC);
	SelectObject(bits, hbtmp);
	BitBlt(hDC, 0, 0, b_width, b_height, bits, 0, 0, SRCCOPY);
	}
	
	write();

	return 0;
}