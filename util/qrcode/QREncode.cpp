// QREncode.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SimpleImage.h"
#include "..\wallet\util\qrcode\QREncode.h"
#include "..\wallet\util\h\define.h"
#include <iostream>
/**
* qrencode - QR Code encoder
*
* QR Code encoding tool
* Copyright (C) 2006, 2007, 2008, 2009 Kentaro Fukuchi <fukuchi@megaui.net>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include "png.h"
#include "getopt.h"

#include "config.h"
#include "qrencoder.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgcodecs.hpp>



#include <opencv2\imgproc\imgproc.hpp>

static int casesensitive = 1;
static int eightbit = 0;
static int version = 0;
static int size = 3;
static int margin = 4;
static int structured = 0;
static QRecLevel level = QR_ECLEVEL_L;
static QRencodeMode hint = QR_MODE_8;

static const struct option options[] = {
	{ "help"         , no_argument      , NULL, 'h' },
{ "output"       , required_argument, NULL, 'o' },
{ "level"        , required_argument, NULL, 'l' },
{ "size"         , required_argument, NULL, 's' },
{ "symversion"   , required_argument, NULL, 'v' },
{ "margin"       , required_argument, NULL, 'm' },
{ "structured"   , no_argument      , NULL, 'S' },
{ "kanji"        , no_argument      , NULL, 'k' },
{ "casesensitive", no_argument      , NULL, 'c' },
{ "ignorecase"   , no_argument      , NULL, 'i' },
{ "8bit"         , no_argument      , NULL, '8' },
{ "version"      , no_argument      , NULL, 'V' },
{ NULL, 0, NULL, 0 }
};

static char *optstring = (char*)"ho:l:s:v:m:Skci8V";

static void usage(int help, int longopt)
{
	fprintf(stderr,
		"qrencode version %s\n"
		"Copyright (C) 2006, 2007, 2008, 2009 Kentaro Fukuchi\n", VERSION);
	if (help) {
		if (longopt) {
			fprintf(stderr,
				"Usage: qrencode [OPTION]... [STRING]\n"
				"Encode input data in a QR Code and save as a PNG image.\n\n"
				"  -h, --help   display the help message. -h displays only the help of short\n"
				"               options.\n\n"
				"  -o FILENAME, --output=FILENAME\n"
				"               write PNG image to FILENAME. If '-' is specified, the result\n"
				"               will be output to standard output. If -S is given, structured\n"
				"               symbols are written to FILENAME-01.png, FILENAME-02.png, ...;\n"
				"               if specified, remove a trailing '.png' from FILENAME.\n\n"
				"  -s NUMBER, --size=NUMBER\n"
				"               specify the size of dot (pixel). (default=3)\n\n"
				"  -l {LMQH}, --level={LMQH}\n"
				"               specify error collectin level from L (lowest) to H (highest).\n"
				"               (default=L)\n\n"
				"  -v NUMBER, --symversion=NUMBER\n"
				"               specify the version of the symbol. (default=auto)\n\n"
				"  -m NUMBER, --margin=NUMBER\n"
				"               specify the width of margin. (default=4)\n\n"
				"  -S, --structured\n"
				"               make structured symbols. Version must be specified.\n\n"
				"  -k, --kanji  assume that the input text contains kanji (shift-jis).\n\n"
				"  -c, --casesensitive\n"
				"               encode lower-case alphabet characters in 8-bit mode. (default)\n\n"
				"  -i, --ignorecase\n"
				"               ignore case distinctions and use only upper-case characters.\n\n"
				"  -8, -8bit    encode entire data in 8-bit mode. -k, -c and -i will be ignored.\n\n"
				"  -V, --version\n"
				"               display the version number and copyrights of the qrencode.\n\n"
				"  [STRING]     input data. If it is not specified, data will be taken from\n"
				"               standard input.\n"
			);
		}
		else {
			fprintf(stderr,
				"Usage: qrencode [OPTION]... [STRING]\n"
				"Encode input data in a QR Code and save as a PNG image.\n\n"
				"  -h           display this message.\n"
				"  --help       display the usage of long options.\n"
				"  -o FILENAME  write PNG image to FILENAME. If '-' is specified, the result\n"
				"               will be output to standard output. If -S is given, structured\n"
				"               symbols are written to FILENAME-01.png, FILENAME-02.png, ...;\n"
				"               if specified, remove a trailing '.png' from FILENAME.\n"
				"  -s NUMBER    specify the size of dot (pixel). (default=3)\n"
				"  -l {LMQH}    specify error collectin level from L (lowest) to H (highest).\n"
				"               (default=L)\n"
				"  -v NUMBER    specify the version of the symbol. (default=auto)\n"
				"  -m NUMBER    specify the width of margin. (default=4)\n"
				"  -S           make structured symbols. Version must be specified.\n"
				"  -k           assume that the input text contains kanji (shift-jis).\n"
				"  -c           encode lower-case alphabet characters in 8-bit mode. (default)\n"
				"  -i           ignore case distinctions and use only upper-case characters.\n"
				"  -8           encode entire data in 8-bit mode. -k, -c and -i will be ignored.\n"
				"  -V           display the version number and copyrights of the qrencode.\n"
				"  [STRING]     input data. If it is not specified, data will be taken from\n"
				"               standard input.\n"
			);
		}
	}
}

#define MAX_DATA_SIZE (7090 * 16) /* from the specification */
static char *readStdin(void)
{
	char *buffer;
	int ret;

	buffer = (char *)malloc(MAX_DATA_SIZE);
	if (buffer == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	ret = fread(buffer, 1, MAX_DATA_SIZE, stdin);
	if (ret == 0) {
		fprintf(stderr, "No input data.\n");
		exit(EXIT_FAILURE);
	}
	if (feof(stdin) == 0) {
		fprintf(stderr, "Input data is too large.\n");
		exit(EXIT_FAILURE);
	}

	buffer[ret] = '\0';

	return buffer;
}

static int writePNG(QRcode *qrcode, const char *outfile)
{
	static FILE *fp = NULL; // avoid clobbering by setjmp.
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	unsigned char *row = NULL, *p = NULL, *q = NULL;
	int x = 0, y = 0, xx = 0, yy = 0, bit = 0;
	int realwidth = 0;

	realwidth = (qrcode->width + margin * 2) * size;
	row = (unsigned char *)malloc((realwidth + 7) / 8);
	if (row == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	if (outfile[0] == '-' && outfile[1] == '\0')
	{
		fp = stdout;
	}
	else
	{
		fp = fopen(outfile, "wb");
		if (fp == NULL) {
			fprintf(stderr, "Failed to create file: %s\n", outfile);
			perror(NULL);
			exit(EXIT_FAILURE);
		}
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Failed to initialize PNG writer.\n");
		exit(EXIT_FAILURE);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Failed to initialize PNG write.\n");
		exit(EXIT_FAILURE);
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fprintf(stderr, "Failed to write PNG image.\n");
		exit(EXIT_FAILURE);
	}

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr,
		realwidth, realwidth,
		1,
		PNG_COLOR_TYPE_GRAY,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	/* top margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for (y = 0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	/* data */
	p = qrcode->data;
	for (y = 0; y<qrcode->width; y++) {
		bit = 7;
		memset(row, 0xff, (realwidth + 7) / 8);
		q = row;
		q += margin * size / 8;
		bit = 7 - (margin * size % 8);
		for (x = 0; x<qrcode->width; x++) {
			for (xx = 0; xx<size; xx++) {
				*q ^= (*p & 1) << bit;
				bit--;
				if (bit < 0) {
					q++;
					bit = 7;
				}
			}
			p++;
		}
		for (yy = 0; yy<size; yy++) {
			png_write_row(png_ptr, row);
		}
	}
	/* bottom margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for (y = 0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);
	free(row);

	return 0;
}

static int writeImage(QRcode *qrcode, const char *outfile)
{
	int nStride = ((((qrcode->width*size * 24) + 31) & ~31) >> 3);

	BITMAPINFO Bi;
	memset(&Bi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	Bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Bi.bmiHeader.biWidth = qrcode->width * size;
	Bi.bmiHeader.biHeight = qrcode->width * size;
	Bi.bmiHeader.biPlanes = 1;
	Bi.bmiHeader.biBitCount = 24;
	Bi.bmiHeader.biCompression = BI_RGB;
	Bi.bmiHeader.biSizeImage = nStride * Bi.bmiHeader.biHeight;

	BITMAPFILEHEADER Bfh;
	memset(&Bfh, 0, sizeof(BITMAPFILEHEADER));
	Bfh.bfType = 0x4d42;//bitmap 정의값. 항상 이 값이어야 한다
	Bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	Bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + Bi.bmiHeader.biSizeImage;

	BYTE *pBuf = new BYTE[Bfh.bfSize];
	if (!pBuf)
		return 1;
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//헤더를 채운다
	memcpy(pBuf, &Bfh, sizeof(BITMAPFILEHEADER));
	memcpy(pBuf + sizeof(BITMAPFILEHEADER), &Bi, sizeof(BITMAPINFOHEADER));
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//이미지 데이터(raster)를 채운다
	BYTE *lpData = &pBuf[Bfh.bfSize];

	unsigned char *p = qrcode->data;
	for (int y = 0; y < qrcode->width; y++)
	{
		lpData -= nStride;
		for (int x = 0; x < qrcode->width; x++)
		{
			BYTE clr;
			if ((BYTE)0x01 & (*p))
				clr = (BYTE)0x00;
			else
				clr = (BYTE)0xff;
		
			for (int i = 0; i < 3 * size; i++)
			{
				if ((BYTE)0x01 & (*p))
				{
					//*(lpData + 1 + i * x * size * 3) = clr;


					//*(lpData + 1 + i + x * size * 3) = clr;
					//*(lpData +     i * 3 + size * x) = 0;
				  //*(lpData + 1 + i * 3 + size * x) = 0xFF;
					*(lpData + i + x * size * 3) = clr;
					//*(lpData + 2 + i * 3 + size * x) = 0;
				}
				else
					*(lpData +     i + x * size * 3) = clr;
				//*(lpData +     i * 3 + size * x) = 0x0F;
				//*(lpData + 1 + i * 3 + size * x) = 0xFF;
				//*(lpData + 2 + i * 3 + size * x) = 0x0F;
		
		
				//*(pDestData +     n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
				//*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
				//*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
			}
			p++;
		}
	//	for (int x = 0; x < qrcode->width; x++)
	//	{
	//		BYTE clr;
	//		if ((BYTE)0x01 & (*p))
	//		{
	//			for (int l = 0; l < 8; l++)
	//			{
	//				for (int n = 0; n < 8; n++)
	//				{
	//					*(lpData + n * 3 + size * l) = 0;
	//					*(lpData + 1 + n * 3 + size * l) = 0xff;
	//					*(lpData + 2 + n * 3 + size * l) = 0;
	//				}
	//			}
	//		}
	//		//lpData += 3 * 8;
	//		p++;
	//	}







		for (int yy = 0; yy < size - 1; yy++)
		{
			memcpy(lpData - nStride, lpData, nStride);
			lpData -= nStride;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	CSimpleImage SimpleImage;
	SimpleImage.SaveImageData(pBuf, Bfh.bfSize, CA2T(outfile));

	delete[] pBuf;
	pBuf = NULL;

	return 0;
}

static QRcode *encode(const char *intext)
{
	QRcode *code = NULL;

	if (eightbit) {
		code = QRcode_encodeString8bit(intext, version, level);
	}
	else {
		code = QRcode_encodeString(intext, version, level, hint, casesensitive);
	}

	return code;
}

char *toUtf8(LPCSTR lpszText)
{
	int nByte;
	char *pszRet = NULL;

	nByte = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszText, -1, NULL, 0);
	LPWSTR wstr = new WCHAR[nByte];
	nByte = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszText, -1, wstr, nByte);

	nByte = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, 0, 0, 0, 0);
	pszRet = new char[nByte];
	nByte = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, pszRet, nByte, 0, 0);

	delete[] wstr;
	wstr = NULL;

	if (nByte == 0)
		return NULL;

	return pszRet; //결과값을 받은 함수에서 메모리 해제 해주어야 한다
}

static void qrencode(const char *intext, const char *outfile)
{
	QRcode *qrcode;

	char* ptuf8 = toUtf8(intext);
	if (ptuf8)
	{
		qrcode = encode(ptuf8);
		delete[] ptuf8;
		ptuf8 = NULL;
	}
	else
		return;

	if (qrcode == NULL)
	{
		perror("Failed to encode the input data:");
		exit(EXIT_FAILURE);
	}

	writeImage(qrcode, outfile);

	QRcode_free(qrcode);
}

static QRcode_List *encodeStructured(const char *intext)
{
	QRcode_List *list;

	if (eightbit) {
		list = QRcode_encodeString8bitStructured(intext, version, level);
	}
	else {
		list = QRcode_encodeStringStructured(intext, version, level, hint, casesensitive);
	}

	return list;
}

static void qrencodeStructured(const char *intext, const char *outfile)
{
	QRcode_List *qrlist, *p;
	char filename[FILENAME_MAX];
	char *base, *q, *suffix = NULL;
	int i = 1;

	base = _strdup(outfile);
	if (base == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	if (strlen(base) > 4) {
		q = base + strlen(base) - 4;
		char* pszlower = _strlwr(q);
		if (strcmp(".png", pszlower) == 0) {
			suffix = _strdup(q);
			*q = '\0';
		}
	}

	qrlist = encodeStructured(intext);
	if (qrlist == NULL) {
		perror("Failed to encode the input data:");
		exit(EXIT_FAILURE);
	}

	for (p = qrlist; p != NULL; p = p->next) {
		if (p->code == NULL) {
			fprintf(stderr, "Failed to encode the input data.\n");
			exit(EXIT_FAILURE);
		}
		if (suffix) {
			_snprintf(filename, FILENAME_MAX, "%s-%02d%s", base, i, suffix);
		}
		else {
			_snprintf(filename, FILENAME_MAX, "%s-%02d", base, i);
		}
		writePNG(p->code, filename);
		i++;
	}

	free(base);
	if (suffix) {
		free(suffix);
	}

	QRcode_List_free(qrlist);
}

void qrcode::m_qrcode(const char* qr_text)
{
	
	qrencode(qr_text, QR_File_Name_Temp);

//	cv::Mat qr_image = cv::imread(QR_File_Name_Temp);
//
//	cv::Mat logo_icon = cv::imread(QR_File_Logo);
//
//	cv::Mat logo_edge = cv::imread(QR_File_Edge);
//
//
//
//
//
//
//
//	// RGB 영역 (수정이 필요함)
//	//cv::Mat skin;
//	//
//	//cv::Vec3b with = cv::Vec3b(0, 255, 0);
//	//
//	//// 손 영역 검출 
//	//inRange(qr_image, cv::Scalar(0, 0, 0), cv::Scalar(125, 125, 125), skin);
//	//qr_image.setTo(with, skin);
//
//
//
//
//
//
//
//
//
//
//
//	cv::Mat resized_icon;
//	cv::Mat resized_edge;
//	/*
//	크기 : w, h
//	*/
//	//resize(qr_image, qr_image, cv::Size(QR_File_Size_width, QR_File_Size_height)); // 변경시 icon, edge 위치,크기가 달라질수 있뜸
//	resize(logo_icon, resized_icon, cv::Size(qr_image.cols / 3, qr_image.rows / 3));
//	//resize(logo_edge, resized_edge, cv::Size(qr_image.cols / 9, qr_image.rows / 9));
//	/*
//	좌표 : x, y
//	*/
//	int cols1 = qr_image.cols / 15;
//	int rows1 = qr_image.rows / 15;
//	int cols2 = qr_image.cols - resized_edge.cols - cols1;
//	int rows2 = qr_image.rows / 15;
//	int cols3 = qr_image.cols / 15;
//	int rows3 = qr_image.rows - resized_edge.cols - rows1;
//	/*
//	좌표 : x, y
//	크기 : w, h
//	*/
//	cv::Mat imageROI1 = qr_image(cv::Rect(qr_image.cols / 3, qr_image.rows / 3, resized_icon.cols, resized_icon.rows));
//	//cv::Mat imageROI2 = qr_image(cv::Rect(cols1, rows1, resized_edge.cols, resized_edge.rows));
//	//cv::Mat imageROI3 = qr_image(cv::Rect(cols2, rows2, resized_edge.cols, resized_edge.rows));
//	//cv::Mat imageROI4 = qr_image(cv::Rect(cols3, rows3, resized_edge.cols, resized_edge.rows));
//
//
//	resized_icon.copyTo(imageROI1);
//	//resized_edge.copyTo(imageROI2);
//	//resized_edge.copyTo(imageROI3);
//	//resized_edge.copyTo(imageROI4);
//
//
//		
//	std::vector<int> param_jpg;  //vector이라는 int형 변수 선언. 여러 요소를 추가삽입할 수 있음. 배열같은 것.
//	param_jpg.push_back(cv::IMWRITE_JPEG_QUALITY);  //JPEG 파일로 저장하기 위해
//	param_jpg.push_back(100);       //높은 값일수록 화질 좋음.용량 높음. 0~100. defalut 값은 95)
//
//	//ShowResult("ImageROI(결과영상)", qr_image);

	//cv::imwrite(QR_File_Name, qr_image, param_jpg);
	//remove(QR_File_Name_Temp);
	//cv::waitKey(0);


}
