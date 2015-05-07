#include "EditBox.h"

WinGui::EditBox::EditBox(unsigned int _type){
	type = _type;
	setClassName("RICHEDIT");
	newClassReg(false);
	setType(type);
	memset(&format, NULL, sizeof(CHARFORMAT2));
	format.cbSize = sizeof(CHARFORMAT2);
	text = new char;
}

WinGui::EditBox::~EditBox(){}

void WinGui::EditBox::SelectUrl(bool yn){
	SendMessage(hWnd, EM_AUTOURLDETECT, yn, NULL);
}

void WinGui::EditBox::setBgColor(COLORREF clr){
	SendMessage(hWnd, EM_SETBKGNDCOLOR, NULL, clr);
}

void WinGui::EditBox::setTextColor(COLORREF text_clr){
	format.dwMask = CFM_COLOR;
	format.crTextColor = text_clr;
	SendMessage(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);
}

int WinGui::EditBox::getText(char* to){
	GetWindowText(hWnd, to, GetWindowTextLength(hWnd) + 1);

	return 0;
}

int WinGui::EditBox::setText(char* str){
	SetWindowText(hWnd, str);

	return 0;
}

int WinGui::EditBox::length(){
	return GetWindowTextLength(hWnd);
}