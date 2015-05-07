#pragma once

//#pragma comment(lib, "comctl32.lib")
//#pragma comment(lib, "winmm.lib")


#include <windows.h>
#include <commctrl.h>
#include <zmouse.h>
#include <string>
#include "Menu.h"

#define MSG(x) MessageBox(NULL, x, "", MB_OK)
#define SET_ERR(err_code) err = err_code;
#define BG_TRANSPARENT (COLORREF)NULL
#define PMENU 1001

namespace WinGui{
	#define MAX_WND		32										//������������ ���-�� ����
	#define MAX_EVENT	16										//������������ ���-�� �������
	class Window;												//�������� ����� ������ � ������
	int Init();													//��������� ��������� ����������, ��������
	int Start();												//������ ����� ��������� ���������
	Window* FindObjByHandle(HWND hwnd);							//������� ����� HWND
	int		AddObj(Window* obj);
	void onPaint(WinGui::Window* obj);							//������� ����������� ����
	struct WINFO{												//���������, ��������� ��� ������ ������� �� HWND
		Window *class_list[MAX_WND];							//������ ������� �������� ������ Window
		int n_window;											//������� ���-�� ����������� ����� �������
	};
	void RBDown(WinGui::Window*);
};


typedef void (*WINPROC)(void*);						//������ ������� - ����������� �������

#define Command(proc)		addEvent((WINPROC)proc, WM_COMMAND);
#define LButtonDown(proc)	addEvent((WINPROC)proc, WM_LBUTTONDOWN);
#define RButtonDown(proc)	addEvent((WINPROC)proc, WM_RBUTTONDOWN);
#define LButtonDClik(proc)	addEvent((WINPROC)proc, WM_LBUTTONDBLCLK);
#define RButtonDClik(proc)	addEvent((WINPROC)proc, WM_RBUTTONDBLCLK);

class WinGui::Window{
public:
	Window();
	~Window();
	HWND Create();

	void setClassName(char* _clsname);
	void newClassReg(bool yn);

	void show();
	void hide();
	int setParent(WinGui::Window *pWnd);
	//��� ���a
	int setType(DWORD _type);
	int setExType(DWORD _ex_type);
	void setCursor(LPCSTR cursor_name);
	void CreateMenu();
	//���������� ��������� ���� (�� ������� Create)
	void setTitle(char* _title);
	void TitleLine(bool param = false);
	//��������� � ������ ����
	void position(int x, int y);
	void size(int _width, int _height);

	void background(COLORREF rgb);
	//����� �����������
	int background(char* path, int _width = NULL, int _height = NULL);
	//������������ ���� (�� 0 �� 255)
	int opacity(int alpha);
	//������������� ����� ����
	int setRgn(char* path);
	int setRoundRectRgn(int wh);

	//���������� HWND
	HWND getHWND();

	WPARAM getWParam();
	LPARAM getLParam();
	HBITMAP GetBgImage();

	WinGui::Menu *PopupMenu;

	//�������� ����������� ���������� �������
	int addEvent(WINPROC pProc, unsigned int EventID, void* _param = NULL);

protected:
	virtual int RefreshGraph();

	WNDCLASS *wnd;
	HWND hWnd;

	char clsname[8];

	char title[33];
	int width, height;
	int pos_x, pos_y;
	int b_width, b_height; //������ ���������� �����������

	DWORD wnd_type, wnd_ex_type;
	HWND parent;
	HDC hDC, bits, bCopyDC;
	HBITMAP hbtmp;
	BITMAP btmp;
	UINT ID;
	HMENU menuID;
	PAINTSTRUCT ps;
	WPARAM wParam;
	LPARAM lParam;

	bool isCreate;
	bool cls_reg;
	bool isNoTL;

	struct WINEVENT{
		UINT eType[MAX_EVENT];
		WINPROC proc[MAX_EVENT];
		void* param;
	}*WinEvent;
	int n_event;

	int err;

	//������� ��������� ���������
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

	friend void WinGui::onPaint(WinGui::Window *obj);
};
