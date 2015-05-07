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
	#define MAX_WND		32										//Максимальное кол-во окон
	#define MAX_EVENT	16										//Максимальное кол-во событий
	class Window;												//Основной класс работы с окнами
	int Init();													//обнуление некоторых переменных, структур
	int Start();												//Запуск цикла обработки сообщений
	Window* FindObjByHandle(HWND hwnd);							//Находим обект HWND
	int		AddObj(Window* obj);
	void onPaint(WinGui::Window* obj);							//Функция перерисовки окна
	struct WINFO{												//Структура, созданная для поиска объекта по HWND
		Window *class_list[MAX_WND];							//Массив адресов объектов класса Window
		int n_window;											//Счетчик кол-ва заполненных ячеек массива
	};
	void RBDown(WinGui::Window*);
};


typedef void (*WINPROC)(void*);						//Формат функции - обработчика события

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
	//Тип окнa
	int setType(DWORD _type);
	int setExType(DWORD _ex_type);
	void setCursor(LPCSTR cursor_name);
	void CreateMenu();
	//Установить заголовок окна (до функции Create)
	void setTitle(char* _title);
	void TitleLine(bool param = false);
	//Положение и размер окна
	void position(int x, int y);
	void size(int _width, int _height);

	void background(COLORREF rgb);
	//Вывод изображения
	int background(char* path, int _width = NULL, int _height = NULL);
	//Прозрачность окна (от 0 до 255)
	int opacity(int alpha);
	//Нестандартная форма окна
	int setRgn(char* path);
	int setRoundRectRgn(int wh);

	//Возвращает HWND
	HWND getHWND();

	WPARAM getWParam();
	LPARAM getLParam();
	HBITMAP GetBgImage();

	WinGui::Menu *PopupMenu;

	//Добавить собственные обработчик событий
	int addEvent(WINPROC pProc, unsigned int EventID, void* _param = NULL);

protected:
	virtual int RefreshGraph();

	WNDCLASS *wnd;
	HWND hWnd;

	char clsname[8];

	char title[33];
	int width, height;
	int pos_x, pos_y;
	int b_width, b_height; //Размер выводимого изображения

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

	//Функция обработки сообщений
	static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

	friend void WinGui::onPaint(WinGui::Window *obj);
};
