#include "Button.h"


WinGui::Button::Button(){
	setClassName("wndbtn");
	setCursor(IDC_HAND);
}

int WinGui::Button::text(LPCSTR _text){
	if(_text == NULL) return -1;
	outText = _text;
	write();

	return 0;
}

void WinGui::Button::setTextColor(COLORREF clr){
	text_color = clr;
}

int WinGui::Button::write(){
	if(outText == NULL || *outText == NULL) return -1;
	GetClientRect(hWnd, &text_rect);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, text_color);
	DrawText(hDC, outText, strlen(outText), &text_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	return 0;
}

int WinGui::Button::RefreshGraph(){
	if(hbtmp){
	bits = CreateCompatibleDC(hDC);
	SelectObject(bits, hbtmp);
	BitBlt(hDC, 0, 0, b_width, b_height, bits, 0, 0, SRCCOPY);
	}
	
	write();

	return 0;

