#include "ImageBox.h"

WinGui::ImageBox::ImageBox(){
mem = NULL;
setClassName("imgbox");
}

int WinGui::ImageBox::setImage(char* path){
	hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int file_size = GetFileSize(hFile, 0);
	mem = (void*)GlobalAlloc(GMEM_FIXED, file_size);
	DWORD dw;
	if(ReadFile(hFile, mem, file_size, &dw, NULL) == false)							goto err_ext;

	if(CreateStreamOnHGlobal((HGLOBAL)mem, TRUE, &stream) != S_OK)					goto err_ext;
	if(OleLoadPicture(stream, 0, true, IID_IPicture, (void**)&m_pPicture) != S_OK)	goto err_ext;
	if(m_pPicture->get_Handle((OLE_HANDLE*)(UINT*)&hbtmp) != S_OK)					goto err_ext;

	BITMAP bmp;
    GetObject(hbtmp, sizeof(BITMAP), &bmp);
	b_width = bmp.bmWidth;
	b_height = bmp.bmHeight;
   
	memDC = CreateCompatibleDC(hDC);
	SelectObject(memDC, hbtmp);

	//stretch_img(width, height);

	FreeResource(mem);
	return 0;

err_ext: 
	FreeResource(mem);
	return SET_ERR(-1);
}

int WinGui::ImageBox::stretch_img(int new_width, int new_height){
	if(StretchBlt(memDC, 0, 0, new_width, new_height, memDC, 0, 0, b_width, b_height, SRCCOPY) == false)
		return SET_ERR(-1);

	b_width = new_width; 
	b_height = new_height;

	return 0;
}

int WinGui::ImageBox::size(int _width, int _height){
	int ret = 0;
	if(isCreate == true){
		MoveWindow(hWnd, pos_x, pos_y, _width, _height, true);
		if(hbtmp){
			ret = stretch_img(_width, _height);
		}	
	}

	width = _width;
	height = _height;

	return ret;
}

int WinGui::ImageBox::RefreshGraph(){
	if(hbtmp){
	BitBlt(hDC, 0, 0, b_width, b_height, memDC, 0, 0, SRCCOPY);
	}

	return 0;
}