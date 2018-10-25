#ifndef _SIMPLEIMAGE_H_
#define _SIMPLEIMAGE_H_

#include <GdiPlus.h>
#include <atlstr.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

#define SIMPLE_BMP _T("image/bmp")
#define SIMPLE_JPG _T("image/jpeg")
#define SIMPLE_PNG _T("image/png")
#define SIMPLE_TIF _T("image/tiff")

class CSimpleImage
{
public:
	CSimpleImage();
	~CSimpleImage();

private:
	ULONG_PTR m_gdiplusToken;
	
public:
	BOOL SaveImageData(const BYTE* pImageData, const int nDataLen, LPCTSTR pszOutputFileName);

protected:
	CString GetImageType(CString strImageName);
	int GetEncoderClsid( char* format/*IN*/, CLSID* pClsid/*OUT*/);	
};

#endif