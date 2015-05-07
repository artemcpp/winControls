#include "Menu.h"

WinGui::Menu::Menu(unsigned int _type){
	type = _type;
	nItems = 0;
	nHMenu = -1;
	mID = new unsigned int[MAX_MENU_ITEMS];
	mii = new MENUITEMINFO;
	memset(mID, NULL, sizeof(unsigned int));
	memset(mii, NULL, sizeof(mii));
	
	if(hMenu == NULL)	err = true;
	else				err = false;
	RBLinked = true;

	mii->cbSize = sizeof(MENUITEMINFO);
}

WinGui::Menu::~Menu(){
	delete mID;
	delete mii;
}

HMENU WinGui::Menu::CreateMenu(HMENU *save_to){
	nHMenu++;

	switch(type){
		case MT_POPUP:
			hMenu = CreatePopupMenu();
		break;
		/*
			...
		*/
	}
	if(save_to == NULL) return NULL;
	*save_to = hMenu;

	return hMenu;
}

unsigned int WinGui::Menu::addMenuItem(char* name, unsigned int wID){
	if(err == true || nItems > MAX_MENU_ITEMS)
		return -1;
	
	mii->fMask = MIIM_STATE | MIIM_ID | MIIM_STRING;
	mii->fType = MFT_STRING;
	mii->fState = MFS_ENABLED;
	mii->dwTypeData = name;
	mii->cch = strlen(name);

	if(wID == NULL)
		mID[nItems] = 5000 + nItems;
	else
		mID[nItems] = wID;
	mii->wID = mID[nItems];
	nItems++;
	
	if(InsertMenuItem(hMenu, 1, false, mii) == false)
		return -1;
	

	return mID[nItems - 1];
}

bool WinGui::Menu::show(int x, int y){
	return TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_TOPALIGN, x, y, NULL, hWnd, NULL);
}

void WinGui::Menu::linkWithRightButton(bool yn){
	RBLinked = yn;
}

bool WinGui::Menu::linkWithRightButton(){
	return RBLinked;
}

void WinGui::Menu::setHWND(HWND _hWnd){
	hWnd = _hWnd;
}