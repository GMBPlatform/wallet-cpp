
#include "stdafx.h"
#include "SimpleImage.h"
#include "..\wallet\util\h\define.h"
#include <locale.h>
wchar_t * to_string(char* m_str);
CSimpleImage::CSimpleImage()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);


}

CSimpleImage::~CSimpleImage()
{
	GdiplusShutdown(m_gdiplusToken);
}

//확장자에 따른 이미지 형태를 구한다
CString CSimpleImage::GetImageType(CString strImageName)
{
	strImageName.MakeReverse();
	int nPos = strImageName.Find(_T("."));
	if (nPos < 0)
		return SIMPLE_BMP;

	CString strExtension = strImageName.Left(nPos).MakeReverse().MakeLower();

	if (strExtension == _T("bmp"))
		return SIMPLE_BMP;
	else if (strExtension == _T("jpg"))
		return SIMPLE_JPG;
	else if (strExtension == _T("png"))
		return SIMPLE_PNG;
	else if (strExtension == _T("tif"))
		return SIMPLE_TIF;
	else
		return SIMPLE_BMP;
}

//헤더포함한 이미지 데이터 포맷을 변경하여 파일로 저장
BOOL CSimpleImage::SaveImageData(const BYTE* pImageData, const int nDataLen, LPCTSTR pszOutputFileName)
{
	char ImageData_str[] = ImageData;
	char QR_File_Extension_str[] = QR_File_Extension;
	char QR_File_Name_Temp_str[] = QR_File_Name_Temp;
	CComPtr<IStream> pStream = NULL;
	CComPtr<IStorage> pIStorage = NULL;
	ULONG uWrite = 0;
	HRESULT hr;
	Status stat = Ok;
	///////////////////////////////////////////////////////////////////////
	//1. 데이터를 저장하기 위한 stream 파일 생성
	//저장공간 생성
	hr = StgCreateDocfile(
		NULL,
		STGM_READWRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
		0,
		&pIStorage);

	if (FAILED(hr))
		return NULL;

	
	//stream 생성
	hr = pIStorage->CreateStream(
		to_string(ImageData_str), //stream 이름(필수인자)
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
		0,
		0,
		&pStream);

	if (FAILED(hr))
		return NULL;
	///////////////////////////////////////////////////////////////////////
	//2.
	hr = pStream->Write(pImageData, nDataLen, &uWrite);//Byte형 비트맵 데이터를 stream에 저장
	if (FAILED(hr))
		return FALSE;
	///////////////////////////////////////////////////////////////////////
	//3.
	Gdiplus::Image gImage(pStream);

	CString strFormat = GetImageType(QR_File_Name_Temp_str);

	CLSID ClsId;


	
	GetEncoderClsid(QR_File_Extension_str, &ClsId);
	




	if (strFormat == SIMPLE_JPG)
	{
		//jpg인 경우 압축이 적용된다
		EncoderParameters encoderParameters;
		ULONG             quality = 100;//0 ~ 100 사이 값, 여기서는 Default값 75를 준다

		encoderParameters.Count = 1;
		encoderParameters.Parameter[0].Guid = EncoderQuality;
		encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
		encoderParameters.Parameter[0].NumberOfValues = 1;
		encoderParameters.Parameter[0].Value = &quality;
		stat = gImage.Save(to_string(QR_File_Name_Temp_str), &ClsId, &encoderParameters);

	}
	else
		stat = gImage.Save(to_string(QR_File_Name_Temp_str), &ClsId);//파일로 저장
																 ///////////////////////////////////////////////////////////////////////
	if (stat == Ok)
		return TRUE;
	else
		return FALSE;
}

//format은 "image/jpeg", "image/bmp", "image/gif", "image/tiff", "image/png" 중 1나
int CSimpleImage::GetEncoderClsid( char* format/*IN*/, CLSID* pClsid/*OUT*/)
{
	UINT  num = 0;          // 이미지 인코더의 개수
	UINT  size = 0;         // 이미지 인코더 배열의 바이트 크기 

	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);

	if (size == 0)
		return -1;  //실패

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

	if (pImageCodecInfo == NULL)
		return -1;  //실패

	GetImageEncoders(num, size, pImageCodecInfo);//이미지 인코더 정보를 구함

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, to_string(format)) == 0)//사용자가 요구한 그래픽 형태와 일치하면
		{
			*pClsid = pImageCodecInfo[j].Clsid;//코덱아이디값 설정
			free(pImageCodecInfo);
			return j;  //성공
		}
	}

	free(pImageCodecInfo);//해제
	return -1;  //실패
}
wchar_t * to_string(char* m_str)
{

	//USES_CONVERSION;
	//
	//wchar_t wTemp[256] = L"\0";
	//MultiByteToWideChar(CP_ACP, 0, m_str, m_str.GetLength(), wTemp, 256);







	//wchar_t wstr[256] = {};
	//
	//setlocale(LC_ALL, "korean");
	//if (mblen(m_str, strlen(m_str)) == 2)
	//{
	//	mbstowcs(wstr, (char*)((LPCSTR)m_str), strlen(m_str));
	//
	//}
	//else
	//{
	//	int nLen = m_str.GetLength() + 1;
	//	mbstowcs(wstr, (char*)((LPCSTR)m_str), nLen);
	//}
	LPWSTR lpszW = new WCHAR[MAX_PATH];
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_str, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, m_str, -1, lpszW, nLen);


	return lpszW;
}
