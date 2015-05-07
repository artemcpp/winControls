#pragma once

#include <windows.h>
#include <olectl.h>
#include "Window.h"

namespace WinGui{
	class ImageBox;
};

class WinGui::ImageBox : public WinGui::Window{
public:
	ImageBox();

	int size(int _width, int _height);
	int setImage(char* path);
protected:
	int RefreshGraph();

	int stretch_img(int new_width, int new_height);

	IStream* stream;
	IPicture* m_pPicture;
	HANDLE hFile;
	HDC memDC;

	void* mem;
};

