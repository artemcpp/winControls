#include "Window.h"

struct WinGui::WINFO winfo;

int WinGui::Init(){
	//InitCommonControls();
	HMODULE hm = LoadLibrary("riched32.dll");
	if(hm == NULL)
		return -1;

	memset(&winfo, NULL, sizeof(WINFO));

	return 0;
}

int WinGui::Start(){
	MSG win_msg;

	while(GetMessage(&win_msg, NULL, 0, 0)){
			TranslateMessage(&win_msg);
			DispatchMessage(&win_msg);
	}

	return 0;
}

WinGui::Window::Window(){
	err = 0;

	wnd = new WNDCLASS;
	memset(wnd, NULL, sizeof(WNDCLASS));

	wnd_type = 0;//CS_OWNDC;
	wnd_ex_type = WS_EX_CONTROLPARENT;

	strcpy(clsname, "wnd123");

	wnd->style = CS_HREDRAW | CS_VREDRAW;
	wnd->lpfnWndProc = WndProc;
	wnd->hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd->lpszClassName = clsname;

	//Устанавливаем по умолчанию положение, ширину, высоту окна и обнуляем размеры выводимой картинки
	strcpy(title, "Window");
	pos_x = pos_y = CW_USEDEFAULT;
	width = height = CW_USEDEFAULT;
	b_width = b_height = 0;

	parent = NULL;
	ID = NULL;
	menuID = NULL;
	hbtmp = NULL;
	cls_reg = true;
	isNoTL = false;

	PopupMenu = new WinGui::Menu;

	WinEvent = new WINEVENT;
	n_event = 0;

	//Устанавливаем функцию перерисовки
	addEvent((WINPROC)WinGui::onPaint, WM_PAINT);

	//Добовляем адрес объекта в массив
	WinGui::AddObj(this);
}

WinGui::Window::~Window(){
	DeleteDC(hDC);
	DeleteDC(bits);
	delete WinEvent;
}

HWND WinGui::Window::Create(){
	if(cls_reg == true)
		RegisterClass(wnd);

	hWnd = CreateWindowEx(
		wnd_ex_type,
		clsname, title,
		wnd_type,
		pos_x, pos_y, width, height,
		parent, NULL, GetModuleHandle(NULL), NULL
		);

	if(hWnd == NULL) return NULL;
	
	PopupMenu->setHWND(hWnd);
	if(PopupMenu->linkWithRightButton() == true)
		RButtonDown(WinGui::RBDown);

	if(isNoTL == true)
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & (~WS_CAPTION));

	hDC = GetDC(hWnd);
	isCreate = true;

	return hWnd;
}

void WinGui::Window::show(){
	ShowWindow(hWnd, SW_SHOW);
}

void WinGui::Window::hide(){
	ShowWindow(hWnd, SW_HIDE);
}

void WinGui::Window::setClassName(char* _clsname){
	strcpy(clsname, _clsname);
	wnd->lpszClassName = clsname;
}

void WinGui::Window::newClassReg(bool yn){
	cls_reg = yn;
}

void WinGui::Window::CreateMenu(){
	menuID = CreatePopupMenu();
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mii.fType = MFT_STRING;
	mii.fState = MFS_ENABLED;
	mii.dwTypeData = "Menu Item!";
	mii.cch = strlen("Menu Item!");
	mii.wID = PMENU;
	mii.hSubMenu = NULL;
	bool t = InsertMenuItem(menuID, 1, false, &mii);
	int xy = GetMessagePos();
	TrackPopupMenu(menuID,
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_TOPALIGN,
		LOWORD(xy), HIWORD(xy),
		NULL, hWnd, NULL);

}

int WinGui::Window::setParent(WinGui::Window *pWnd){
	parent = pWnd->getHWND();
	setType(WS_CHILD);
	return 0;
}

int WinGui::Window::setType(DWORD _type){
	wnd_type |= _type;

	return 0;
}

int WinGui::Window::setExType(DWORD _ex_type){
	wnd_ex_type |= _ex_type;

	return 0;
}

void WinGui::Window::setCursor(LPCSTR cursor_name){
	if(isCreate == true)
		SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, cursor_name));
	else
		wnd->hCursor = LoadCursor(NULL, cursor_name);
}

void WinGui::Window::setTitle(char* _title){
	strcpy(title, _title);
}

void WinGui::Window::TitleLine(bool param){
	if(param == false){
		title[0] = NULL;
		wnd_type |= WS_BORDER | WS_VISIBLE;
		isNoTL = true;
	}
}

void WinGui::Window::position(int x, int y){
	pos_x = x;
	pos_y = y;

	if(isCreate == true)
		MoveWindow(hWnd, pos_x, pos_y, width, height, true);
}

void WinGui::Window::size(int _width, int _height){
	width = _width;
	height = _height;

	if(isCreate == true)
		MoveWindow(hWnd, pos_x, pos_y, width, height, true);
}

void WinGui::Window::background(COLORREF rgb){
	HBRUSH brush;
	if(rgb == BG_TRANSPARENT){
	brush = NULL;
	}else{
	brush = CreateSolidBrush(rgb);
	}

	if(isCreate == true)
		SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG)brush);
	else
		wnd->hbrBackground = (HBRUSH)brush;
}

int WinGui::Window::background(char* path, int _width, int _height){
	if(_width == NULL){
		b_width = width;
		b_height = height;
	}else{
		b_width = _width;
		b_height = _height;
	}
	
	hbtmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	if(hbtmp == NULL)
		return -1;

	RefreshGraph();

	return 0;
}

int WinGui::Window::setRgn(char* path){
	BITMAP bi;
	BYTE bpp;
	DWORD delPxl;
	DWORD pixel;
	int startx;
	int i, j;
	int arraySize;

	HRGN Rgn, ResRgn = CreateRectRgn(0, 0, 0, 0);

	hbtmp = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//hbtmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(path), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if(hbtmp == NULL)
		return -1;

	GetObject(hbtmp, sizeof(BITMAP), &bi);
	bpp = bi.bmBitsPixel >> 3;
	arraySize = bi.bmWidth * bi.bmHeight * bpp;

	BYTE *pBits = new BYTE[arraySize];

	int p = GetBitmapBits(hbtmp, arraySize, pBits);

	delPxl = *(DWORD*)pBits;
	delPxl <<= 32 - bi.bmBitsPixel;

	for (i = 0; i < bi.bmHeight; i++){
		startx = -1;
		for (j = 0; j < bi.bmWidth; j++){
			pixel = *(DWORD*)(pBits + (i * bi.bmWidth + j) * bpp) << (32 - bi.bmBitsPixel);
			if (pixel != delPxl){
				if (startx < 0){
					startx = j;
				}else if(j == (bi.bmWidth - 1)){
					Rgn = CreateRectRgn(startx, i, j, i + 1);
					CombineRgn(ResRgn, ResRgn, Rgn, RGN_OR);
					startx = -1;
				}
			}else if (startx >= 0){
				Rgn = CreateRectRgn(startx, i, j, i + 1);
				CombineRgn(ResRgn, ResRgn, Rgn, RGN_OR);
				startx = -1;
			}
		}
	}

	SetWindowRgn(hWnd, ResRgn, true);
	InvalidateRect(hWnd, 0, false);

	delete pBits;

	return 0;
}

int WinGui::Window::setRoundRectRgn(int wh){
	HRGN rrRgn = CreateRoundRectRgn(0, 0, width, height, wh, wh);

	SetWindowRgn(hWnd, rrRgn, true);
	InvalidateRect(hWnd, 0, false);

	return 0;
}

int WinGui::Window::opacity(int alpha){
	SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);

	return 0;
}

HWND WinGui::Window::getHWND(){
	return hWnd;
}

WPARAM WinGui::Window::getWParam(){
	return wParam;
}

LPARAM WinGui::Window::getLParam(){
	return lParam;
}

HBITMAP WinGui::Window::GetBgImage(){
	return hbtmp;
}

int WinGui::Window::RefreshGraph(){
	if(hbtmp){
	bits = CreateCompatibleDC(hDC);
	SelectObject(bits, hbtmp);
	BitBlt(hDC, 0, 0, b_width, b_height, bits, 0, 0, SRCCOPY);
	}

	return 0;
}

int WinGui::Window::addEvent(WINPROC pProc, unsigned int EventID, void* _param){
	if(n_event < MAX_EVENT){
		WinEvent->eType[n_event] = EventID;
		WinEvent->proc[n_event] = pProc;
		if(_param == NULL)
			WinEvent->param = this;
		else
			WinEvent->param = _param;
		n_event++;
	}else{
		return -1;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////

WinGui::Window* WinGui::FindObjByHandle(HWND hwnd){
	int i = 0;
	while(winfo.class_list[i] != NULL){
		if(winfo.class_list[i]->getHWND() == hwnd) break;
		i++;
	}
	return winfo.class_list[i];
}

int WinGui::AddObj(Window* obj){
	if(winfo.n_window < MAX_WND){
		winfo.class_list[winfo.n_window] = obj;
 		winfo.n_window++;
	}else{
		return -1;
	}

	return 0;
}


LRESULT WINAPI WinGui::Window::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
	int i = 0;
	int j = 0;
	WinGui::Window *pWindow;
	
	//Ищем класс по hwnd
	pWindow = WinGui::FindObjByHandle(hwnd);

	if(pWindow == NULL)
		return DefWindowProc(hwnd, message, wparam, lparam);

	//Ищем функцию, привязанную к полученному событию
	while(pWindow->WinEvent->eType[j] != message){
		if(j == MAX_EVENT)
			return DefWindowProc(hwnd, message, wparam, lparam);
		j++;
	}

	pWindow->wParam = wparam;
	pWindow->lParam = lparam;

	//Выполняем нужную функцию 
	pWindow->WinEvent->proc[j]((void*)pWindow);

	return DefWindowProc(hwnd, message, wparam, lparam);
}

//Заранее определённые функции обработки сообщений:

void WinGui::onPaint(WinGui::Window *obj){
	obj->hDC = BeginPaint(obj->hWnd, &obj->ps);

	obj->RefreshGraph();

	DeleteDC(obj->hDC);
	DeleteDC(obj->bits);
	EndPaint(obj->hWnd, &obj->ps);
}

void WinGui::RBDown(WinGui::Window* wnd){
	WPARAM xy = GetMessagePos();
	bool q = wnd->PopupMenu->show(LOWORD(xy), HIWORD(xy));
}
