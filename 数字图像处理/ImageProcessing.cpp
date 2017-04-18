// ImageProcessing.cpp : Defines the entry point for the application.
//

#include"stdafx.h"
#include"resource.h"
#include<direct.h>
#include<commdlg.h>
#include<ctime>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<vector>
using namespace std;
#define MAX_LOADSTRING 100

BOOL ReadImage(LPSTR, char *, int, int); //��ȡͼ����Ϣ��������Image[][]��
void ShowImage(char *, int, int, int, int);						//��ʾRAW�ļ�
BOOL ReadBmpImage(LPSTR, char *);								//��ȡBMP�ļ�
void ShowBmpImage(char *, int, int, int, int);					//��ʾBMP�ļ�
void OpenImageFileDlg(char *);									//��ͼ���ļ�
void ShowImageRect(char *, int, int, int, int, RECT, int);		//��ʾ����ͼ��
void ShowRawDoor(char *, int, int, int, int, int);				//Raw��/������Ч 
void ShowRawShutter(char *, int, int, int, int, int);			//Raw��Ҷ����Ч 
void ShowRawFade(char *, int, int, int, int, int);				//Raw���뵭����Ч
void ShowBmpDoor(char *, int, int, int, int, int);				//Bmp��/������Ч 
void ShowBmpShutter(char *, int, int, int, int, int);			//Bmp��Ҷ����Ч
void ShowBmpFade(char *, int, int, int, int, int);				//Bmp���뵭����Ч
void ImageHistogramShowProcessing(char *, char *, int, int);	//��ʾֱ��ͼ����
void ImageGrayReversionProcessing(char *, char*, int, int);		//ͼ��ת����
void ThresholdImage(char *, char*, int, int);					//ͼ���ֵ������
void ImageMaskProcessing(char *, char *, int, int, int *, int, int, bool); //3x3ģ�崦����
void AverageMaskProcessing(char *, char *, int, int);			//��ֵ�˲�����
void GuassAverageMaskProcessing(char *, char *, int, int);		//��˹(ƽ��)�˲�����
void OrderStaticFilterProcessing(char *, char *, int, int, int, int);	//ͳ�������˲�����
void EdgeDectectProcessing(char *, char *, int, int, int);		//��Ե����˲�����
void ImageScaling(char *, char *, int, int, double, double);			//ͼ�����ű任����
void ImageTranslation(char *, char *, int, int, int, int);				//ͼ��ƽ�Ʊ任����
void SimpleGeometricTranslation(char *, char *, int, int, double *);	//ͼ��򵥼��α任����
void ImageRotation(char *, char *, int, int, double);					//ͼ����ת�任����

HDC  hWinDC;
HWND hWind;
int fileType;
int ImageWidth, ImageHeight;
char ImgDlgFileName[MAX_PATH];
char ImgDlgFileDir[MAX_PATH];
char OrgImage[1024*1024];
char NewImage[1024 * 1024];	//�µ�ͼ�񻺴�
#define IMAGEWIDTH	256		//rawͼ����
#define IMAGEHEIGHT	256		//rawͼ��߶�
#define XPOS		100		//��ʾ����ԭ��
#define YPOS		100		//��ʾ����ԭ��
#define SHUTTERNUM 5		//��Ҷ������
#define GRAYCHANGENUM 5		//���뵭�����
#define FADEIN 1			//����
#define FADEOUT 0			//����
#define LEFTRIGHTOPEN 0		//�м������ң����ţ�
#define LEFTRIGHTCLOSE 1	//�������м�(���ţ�
#define TOPBOTTOMOPEN 2		//�м������£����ţ�
#define TOPBOTTOMCLOSE 3	//�������м䣨���ţ�
#define TORIGHTSHUTTER 0	//�������Ұ�Ҷ��
#define TOLEFTSHUTTER 1		//���������Ҷ��
#define TOBOTTOMSHUTTER 2	//�������°�Ҷ��
#define TOTOPSHUTTER 3		//�������ϰ�Ҷ��
#define RAW 0
#define BMP 1
#define HISTOGRAMHIEGHT	150	//��ʾֱ��ͼ����
#define MAXFILTER 0			//���ֵ�˲�
#define MINFILTER 1			//��Сֵ�˲�
#define MIDFILTER 2			//��ֵ�˲�
#define EDGELAPLACE 0		//Laplace��Ե���
#define EDGESOBEL 1			//Sobel��Ե���
#define EDGEPREWITT 2		//Prewitt��Ե���
#define EDGEROBERTS 3		//Roberts��Ե���


int iThreshold = 128;		//ͼ���ֵ������

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGEPROCESSING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGEPROCESSING);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGEPROCESSING);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGEPROCESSING;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_CREATE:
			hWind = hWnd;
			hWinDC = GetWindowDC(hWnd);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_OPEN:		//��
					OpenImageFileDlg("��");
					if (fileType == RAW)
					{
						ReadImage(ImgDlgFileName, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
						ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
					}
					else if (fileType == BMP)
					{
						ReadBmpImage(ImgDlgFileName, OrgImage);
						ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
					}
					break;
				case IDM_FADEIN:	//����
					if (fileType == RAW)
						ShowRawFade(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, FADEIN);
					else if (fileType == BMP)
						ShowBmpFade(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, FADEIN);
					break;
				case IDM_FADEOUT:	//����
					if (fileType == RAW)
						ShowRawFade(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, FADEOUT);
					else if (fileType == BMP)
						ShowBmpFade(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, FADEOUT);
					break;
				case IDM_LEFTRIGHTOPEN:		//�м������ң����ţ�
					if (fileType == RAW)
						ShowRawDoor(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, LEFTRIGHTOPEN);
					else if (fileType == BMP)
						ShowBmpDoor(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, LEFTRIGHTOPEN);
					break;
				case IDM_LEFTRIGHTCLOSE:	//�������м�(���ţ�
					if (fileType == RAW)
						ShowRawDoor(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, LEFTRIGHTCLOSE);
					else if (fileType == BMP)
						ShowBmpDoor(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, LEFTRIGHTCLOSE);
					break;
				case IDM_TOPBOTTOMOPEN:		//�м������£����ţ�
					if (fileType == RAW)
						ShowRawDoor(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, TOPBOTTOMOPEN);
					else if (fileType == BMP)
						ShowBmpDoor(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, TOPBOTTOMOPEN);
					break;
				case IDM_TOPBOTTOMCLOSE:    //�������м䣨���ţ�
					if (fileType == RAW)
						ShowRawDoor(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, TOPBOTTOMCLOSE);
					else if (fileType == BMP)
						ShowBmpDoor(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, TOPBOTTOMCLOSE);
					break;
				case IDM_TORIGHTSHUTTER:	//�������Ұ�Ҷ��
					if (fileType == RAW)
						ShowRawShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, TORIGHTSHUTTER);
					else if (fileType == BMP)
						ShowBmpShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, TORIGHTSHUTTER);
					break;
				case IDM_TOLEFTSHUTTER:		//���������Ҷ��
					if (fileType == RAW)
						ShowRawShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, TOLEFTSHUTTER);
					else if (fileType == BMP)
						ShowBmpShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, TOLEFTSHUTTER);
					break;
				case IDM_TOBUTTOMSHUTTER:	//�������°�Ҷ��
					if (fileType == RAW)
						ShowRawShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, TOBOTTOMSHUTTER);
					else if (fileType == BMP)
						ShowBmpShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, TOBOTTOMSHUTTER);
					break;
				case IDM_TOTOPSHUTTER:		//�������ϰ�Ҷ��
					if (fileType == RAW)
						ShowRawShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS, TOTOPSHUTTER);
					else if (fileType == BMP)
						ShowBmpShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS, TOTOPSHUTTER);
					break;
				case IDM_HISTOGRAM:			//ͼ��ֱ��ͼ
					ImageHistogramShowProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_THRESHOLD:			//ͼ���ֵ��
					ThresholdImage(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_GRAYREVERSION:		//�Ҷȷ�ת
					ImageGrayReversionProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_AVERAGEFILTER:		//ƽ���˲���
					AverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_AVERAGEGAUSS:		//��˹�˲���
					GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_MAXFILTER:			//���ֵ�˲�
					OrderStaticFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 3, MAXFILTER);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_MINFILTER:			//��Сֵ�˲�
					OrderStaticFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 3, MINFILTER);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_MIDFILTER:			//��ֵ�˲�
					OrderStaticFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 3, MIDFILTER);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300); break;
				case IDM_EDGELAPLACE:		//Laplace��Ե���
					EdgeDectectProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, EDGELAPLACE);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
				case IDM_EDGESOBEL:		//Sobel��Ե���
					EdgeDectectProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, EDGESOBEL);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
				case IDM_EDGEPREWITT:		//Prewitt��Ե���
					EdgeDectectProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, EDGEPREWITT);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
				case IDM_EDGEROBERTS:		//Roberts��Ե���
					EdgeDectectProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, EDGEROBERTS);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
				case IDM_TRANSLATION:				//ͼ��ƽ��
					ImageTranslation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 20, 30);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);  break;
				case IDM_SCALING:					//ͼ������
					ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 0.5);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
				case IDM_ROTATION:					//ͼ����ת
					ImageRotation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 45.0);
					ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);	 break;
				case IDM_ABOUT:		//����
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:		//�˳�
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

/************************************************************************************************
*																								*
*   ���޸�ʽRAWͼ���ļ�																			*
*																								*
************************************************************************************************/
BOOL ReadImage(LPSTR ImageFileName, char *oImage, int wImage, int hImage) //��ȡͼ����Ϣ��������Image[][]��
{
	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR) 
	{
		MessageBox(NULL, ImageFileName, "���ļ�������Ϣ", MB_OK);
		return FALSE;
	}

	_lread(Image_fp, oImage, wImage*hImage);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
*																								*
*   ��ʾRAWͼ��																					*
*																								*
************************************************************************************************/
void ShowImage(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i,j;

	for (i=0; i<hImage; i++) {
		for (j=0; j<wImage; j++) {
			SetPixel(hWinDC, j+yPos, i+xPos, RGB(Image[i*wImage+j],Image[i*wImage+j],Image[i*wImage+j]));
		}
	}
}

/************************************************************************************************
*																								*
*   ��BMPͼ���У�����ͼ��ͷ��Ϣ����Ҫ����ͼ�񳤶ȺͿ��											*
*																								*
************************************************************************************************/
BOOL ReadBmpImage(LPSTR ImageFileName, char *oImage)
{ 
	BITMAPFILEHEADER bfImage;
	BITMAPINFOHEADER biImage;

	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR) 
	{
		MessageBox(NULL, ImageFileName, "���ļ�������Ϣ", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, 0, 0);
	_lread(Image_fp, &bfImage, sizeof(BITMAPFILEHEADER));

	if ((bfImage.bfType != 0x4d42)) {		// "BM"
		MessageBox(NULL, NULL, "��bmp�ļ�������Ϣ", MB_OK);
		return FALSE;
	}

	_lread(Image_fp, &biImage, sizeof(BITMAPINFOHEADER));

	ImageWidth = biImage.biWidth;
	ImageHeight= biImage.biHeight;
	if (biImage.biBitCount != 24) {		// 24λ��ɫͼ��
		MessageBox(NULL, NULL, "����24λbmpͼ��", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, bfImage.bfOffBits, 0);
	_lread(Image_fp, oImage, biImage.biWidth*biImage.biHeight*3);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
*																								*
*   ��ʾBMPͼ��																					*
*																								*
************************************************************************************************/
void ShowBmpImage(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i,j;
	for (i=0; i<hImage; i++) {
		for (j=0; j<wImage; j++) {
			SetPixel(hWinDC, j + yPos, hImage - 1 - i + xPos, RGB(Image[i*wImage * 3 + j * 3 + 2], Image[i*wImage * 3 + j * 3 + 1], Image[i*wImage * 3 + j * 3 + 0]));
		}
	}
}
/************************************************************************************************
*																								*
*   �򿪶��ļ���ϵͳ�Ի���																		*
*																								*
************************************************************************************************/
void OpenImageFileDlg(char *OPDLTitle)
{
	char FileTitle[100];
	OPENFILENAME ofn;

	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=NULL;
	ofn.hInstance=NULL;
	ofn.lpstrFilter = "����ͼƬ�ļ�\0*.bmp;*.raw\0�����ļ�\0*.*\0\0";
	ofn.lpstrCustomFilter=NULL;
	ofn.nMaxCustFilter=0;
	ofn.nFilterIndex=1;
	ofn.lpstrFile=ImgDlgFileName;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFileTitle=FileTitle;
	ofn.nMaxFileTitle=99;
	ofn.lpstrInitialDir=ImgDlgFileDir;
	ofn.lpstrTitle=OPDLTitle;
	ofn.Flags=OFN_FILEMUSTEXIST;
	ofn.lCustData=NULL;
	ofn.lpfnHook=NULL;
	ofn.lpTemplateName=NULL;
	ImgDlgFileName[0]='\0';
	GetOpenFileName(&ofn); 
	int t = ImgDlgFileName[ofn.nFileExtension];
	if (t == 'r')
		fileType = RAW;
	else if (t == 'b')
		fileType = BMP;
	getcwd(ImgDlgFileDir, MAX_PATH);
}

void ShowImageRect(char *Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect, int type)
{
	int i, j, r, g, b;
	for (i = ShowRect.top; i < ShowRect.bottom; i++)
	{
		for (j = ShowRect.left; j < ShowRect.right; j++)
		{
			switch(type)
			{
				case RAW:
					r = g = b = (BYTE)Image[i*wImage + j]; SetPixel(hWinDC, j + yPos, i + xPos, RGB(r, g, b));
					break;
				case BMP:
					r = Image[i*wImage * 3 + j * 3 + 2]; g = Image[i*wImage * 3 + j * 3 + 1]; b = Image[i*wImage * 3 + j * 3 + 0];
					SetPixel(hWinDC, j + yPos, hImage - 1 - i + xPos, RGB(r, g, b));
					break;
			}
		}
	}
}

//Raw��/������Ч
void ShowRawDoor(char *Image, int wImage, int hImage, int xPos, int yPos, int type)
{
	int i, iEdge; RECT ShowRect;
	switch (type)
	{
	case LEFTRIGHTOPEN:
	case LEFTRIGHTCLOSE:
		ShowRect.top = 0; ShowRect.bottom = hImage; iEdge = wImage / 2; break;
	case TOPBOTTOMOPEN:
	case TOPBOTTOMCLOSE:
		ShowRect.left = 0; ShowRect.right = wImage; iEdge = hImage / 2; break;
	}
	InvalidateRgn(hWind, NULL, TRUE);	UpdateWindow(hWind);	// ��մ���
	for (i = 0; i <iEdge; i++) 
	{
		switch(type)
		{
		case LEFTRIGHTOPEN:		//��ʾһ��
			ShowRect.left = wImage / 2 - i; ShowRect.right = ShowRect.left + 1; break;
		case LEFTRIGHTCLOSE:	//��ʾһ��
			ShowRect.left = i; ShowRect.right = ShowRect.left + 1; break;
		case TOPBOTTOMOPEN:		//��ʾһ��
			ShowRect.top = hImage / 2 - i; ShowRect.bottom = ShowRect.top + 1; break;
		case TOPBOTTOMCLOSE:	//��ʾһ��
			ShowRect.top = i; ShowRect.bottom = ShowRect.top + 1; break;
		}
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect, RAW);	//����
		switch (type)
		{
		case LEFTRIGHTOPEN:		//��ʾһ��
			ShowRect.left = wImage / 2 + i; ShowRect.right = ShowRect.left + 1; break;
		case LEFTRIGHTCLOSE:	//��ʾһ��
			ShowRect.right = wImage - 1 - i; ShowRect.left = ShowRect.right - 1; break;
		case TOPBOTTOMOPEN:		//��ʾһ��
			ShowRect.top = hImage / 2 + i; ShowRect.bottom = ShowRect.top + 1; break;
		case TOPBOTTOMCLOSE:	//��ʾһ��
			ShowRect.bottom = hImage - 1 - i; ShowRect.top = ShowRect.bottom - 1; break;
		}
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect, RAW);	//�ҿ���
		Sleep(1);
	}
}

//Raw��Ҷ����Ч
void ShowRawShutter(char *Image, int wImage, int hImage, int xPos, int yPos, int type)
{
	int i, j, iEdge;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //��մ���
	switch (type)
	{
	case TORIGHTSHUTTER:
	case TOLEFTSHUTTER:
		ShowRect.top = 0; ShowRect.bottom = wImage; iEdge = hImage / SHUTTERNUM; break;
	case TOBOTTOMSHUTTER:
	case TOTOPSHUTTER:
		ShowRect.left = 0; ShowRect.right = hImage; iEdge = wImage / SHUTTERNUM; break;
	}
	for (i = 0; i <= iEdge; i++) // < ��Ϊ <=
	{	 //SHUTTERNUM�ǰ�Ҷ������
		for (j = 0; j < SHUTTERNUM; j++) 
		{
			switch (type)
			{
			case TORIGHTSHUTTER:	//��ʾһ��
				ShowRect.left = i + (j*hImage / SHUTTERNUM); ShowRect.right = ShowRect.left + 1; break;
			case TOLEFTSHUTTER:
				ShowRect.left = iEdge - i + (j*hImage / SHUTTERNUM); ShowRect.right = ShowRect.left + 1; break;
			case TOBOTTOMSHUTTER:
				ShowRect.top = i + (j*wImage / SHUTTERNUM); ShowRect.bottom = ShowRect.top + 1; break;
			case TOTOPSHUTTER:
				ShowRect.top = iEdge - i + (j*wImage / SHUTTERNUM); ShowRect.bottom = ShowRect.top + 1; break;
			}
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect, RAW);
			Sleep(1);
		}
	}
}

//Raw���뵭����Ч
void ShowRawFade(char *Image, int wImage, int hImage, int xPos, int yPos, int type)
{
	int i, j, m, r, g, b, gray;
	if (type == FADEIN)
	{
		InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //��մ���
	}
	for (m = 1; m <= GRAYCHANGENUM; m++)
	{
		for (i = 0; i < hImage; i++)
		{
			for (j = 0; j < wImage; j++)
			{
				gray = (BYTE)Image[i*wImage + j];
				if (type == FADEIN)
					r = g = b = gray + (255 - gray)*(GRAYCHANGENUM - m) / GRAYCHANGENUM;
				else
					r = g = b = gray + (255 - gray)*m / GRAYCHANGENUM;
				SetPixel(hWinDC, j + yPos, i + xPos, RGB(r, g, b));
			}
		}
	}
}

//Bmp��/������Ч
void ShowBmpDoor(char *Image, int wImage, int hImage, int xPos, int yPos, int type)
{
	int i, iEdge; RECT ShowRect;
	switch (type)
	{
	case LEFTRIGHTOPEN:
	case LEFTRIGHTCLOSE:
		ShowRect.top = 0; ShowRect.bottom = hImage; iEdge = wImage / 2; break;
	case TOPBOTTOMOPEN:
	case TOPBOTTOMCLOSE:
		ShowRect.left = 0; ShowRect.right = wImage; iEdge = hImage / 2; break;
	}
	InvalidateRgn(hWind, NULL, TRUE);	UpdateWindow(hWind);	// ��մ���
	for (i = 0; i < iEdge; i++)
	{
		switch (type)
		{
		case LEFTRIGHTOPEN:		//��ʾһ��
			ShowRect.left = wImage / 2 - i; ShowRect.right = ShowRect.left + 1; break;
		case LEFTRIGHTCLOSE:	//��ʾһ��
			ShowRect.left = i; ShowRect.right = ShowRect.left + 1; break;
		case TOPBOTTOMOPEN:		//��ʾһ��
			ShowRect.top = hImage / 2 - i; ShowRect.bottom = ShowRect.top + 1; break;
		case TOPBOTTOMCLOSE:	//��ʾһ��
			ShowRect.top = i; ShowRect.bottom = ShowRect.top + 1; break;
		}
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect, BMP);	//����
		switch (type)
		{
		case LEFTRIGHTOPEN:		//��ʾһ��
			ShowRect.left = wImage / 2 + i; ShowRect.right = ShowRect.left + 1; break;
		case LEFTRIGHTCLOSE:	//��ʾһ��
			ShowRect.right = wImage - 1 - i; ShowRect.left = ShowRect.right - 1; break;
		case TOPBOTTOMOPEN:		//��ʾһ��
			ShowRect.top = hImage / 2 + i; ShowRect.bottom = ShowRect.top + 1; break;
		case TOPBOTTOMCLOSE:	//��ʾһ��
			ShowRect.bottom = hImage - 1 - i; ShowRect.top = ShowRect.bottom - 1; break;
		}
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect, BMP);	//�ҿ���
		Sleep(1);
	}
}

//Bmp��Ҷ����Ч
void ShowBmpShutter(char *Image, int wImage, int hImage, int xPos, int yPos, int type)
{
	int i, j, iEdge;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //��մ���
	switch (type)
	{
	case TORIGHTSHUTTER:
	case TOLEFTSHUTTER:
		ShowRect.top = 0; ShowRect.bottom = hImage; iEdge = wImage / SHUTTERNUM; break;
	case TOBOTTOMSHUTTER:
	case TOTOPSHUTTER:
		ShowRect.left = 0; ShowRect.right = wImage; iEdge = hImage / SHUTTERNUM; break;
	}
	for (i = 0; i <= iEdge; i++) // < ��Ϊ <=
	{	 //SHUTTERNUM�ǰ�Ҷ������
		for (j = 0; j < SHUTTERNUM; j++)
		{
			switch (type)
			{
			case TORIGHTSHUTTER:	//��ʾһ��
				ShowRect.left = i + (j*wImage / SHUTTERNUM); ShowRect.right = ShowRect.left + 1; break;
			case TOLEFTSHUTTER:
				ShowRect.left = iEdge - i + (j*wImage / SHUTTERNUM); ShowRect.right = ShowRect.left + 1; break;
			case TOBOTTOMSHUTTER:
				ShowRect.top = iEdge - i + (j*hImage / SHUTTERNUM); ShowRect.bottom = ShowRect.top + 1; break;
			case TOTOPSHUTTER:
				ShowRect.top = i + (j*hImage / SHUTTERNUM); ShowRect.bottom = ShowRect.top + 1; break;
			}
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect, BMP);
			Sleep(1);
		}
	}
}

//Bmp���뵭����Ч
void ShowBmpFade(char *Image, int wImage, int hImage, int xPos, int yPos, int type)
{
	int i, j, m, r, g, b, rOri, gOri, bOri;
	if (type == FADEIN)
	{
		InvalidateRgn(hWind, NULL, TRUE); UpdateWindow(hWind);     //��մ���
	}
	for (m = 1; m <= GRAYCHANGENUM; m++)
	{
		for (i = 0; i < hImage; i++)
		{
			for (j = 0; j < wImage; j++)
			{
				rOri = (BYTE)Image[i*wImage * 3 + j * 3 + 2];
				gOri = (BYTE)Image[i*wImage * 3 + j * 3 + 1];
				bOri = (BYTE)Image[i*wImage * 3 + j * 3 + 0];
				if (type == FADEIN)
				{
					r = rOri + (255 - rOri)*(GRAYCHANGENUM - m) / GRAYCHANGENUM;
					g = gOri + (255 - gOri)*(GRAYCHANGENUM - m) / GRAYCHANGENUM;
					b = bOri + (255 - bOri)*(GRAYCHANGENUM - m) / GRAYCHANGENUM;
				}
				else
				{
					r = rOri + (255 - rOri)*m / GRAYCHANGENUM;
					g = gOri + (255 - gOri)*m / GRAYCHANGENUM;
					b = bOri + (255 - bOri)*m / GRAYCHANGENUM;
				}
				SetPixel(hWinDC, j + yPos, hImage - 1 - i + xPos, RGB(r, g, b));
			}
		}
	}
}

//��ʾֱ��ͼ����
void ImageHistogramShowProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int i, j, Gray, MaxGrayCount; int GrayCount[256];
	for (i = 0; i < 256; i++) 
		GrayCount[i] = 0;
	for (i = 0; i < wImage*hImage; i++)
	{
		Gray = (BYTE)oImage[i];
		GrayCount[Gray]++;
	}
	MaxGrayCount = GrayCount[0];
	for (i = 0; i < 256; i++)
		if (MaxGrayCount < GrayCount[i])
			MaxGrayCount = GrayCount[i];
	for (i = 0; i < 256 * 256; i++)
		nImage[i] = (BYTE)192;
	for (j = 0; j < 256; j++)
	{
		GrayCount[j] = (int)(GrayCount[j] * HISTOGRAMHIEGHT / MaxGrayCount);
		for (i = 0; i < GrayCount[j]; i++) 
			nImage[(255 - i) * 256 + j] = 0;
	}
}

//ͼ��Ҷȷ�ת����
void ImageGrayReversionProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	for (int i = 0; i < wImage*hImage; i++)
		nImage[i] = (BYTE)(255 - (BYTE)oImage[i]);
}

//ͼ���ֵ������
void ThresholdImage(char *oImage, char *nImage, int wImage, int hImage)
{
	for (int i = 0; i < wImage*hImage; i++)
		if ((BYTE)oImage[i] < iThreshold)
			nImage[i] = 0;
		else
			nImage[i] = (BYTE)255;
}

//3x3ģ�崦����
void ImageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage, int *Mask, int MaskWH, int MaskCoff, bool DBL = false)
{
	int Coff, i, j, m, n, k;
	k = (MaskWH - 1) / 2;
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			Coff = 0;
			for (m = -k; m <= k; m++)
			{
				for (n = -k; n <= k; n++)
				{
					Coff += (BYTE)oImage[(i + m)*wImage + (j + n)] * Mask[(m + k)*MaskWH + (n + k)];
					if (DBL)	//����ģ��
						Coff += (BYTE)oImage[(i + m)*wImage + (j + n)] * Mask[(m + k)*MaskWH + (n + k) + MaskWH*MaskWH];
				}
			}
			nImage[i*wImage + j] = (unsigned char)(Coff / MaskCoff);
			if (Coff < 0)
				nImage[i*wImage + j] = 0;
		}
	}
}

//��ֵ�˲�����
void AverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Mask[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 9);
}

//��˹(ƽ��)�˲�����
void GuassAverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Mask[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 16);
}

//��Ե����˲�����
void EdgeDectectProcessing(char *oImage, char *nImage, int wImage, int hImage, int type)
{
	if (type == EDGELAPLACE)	//Laplace��Ե���
	{
		int Mask[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
		ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 1);
	}
	else if (type == EDGESOBEL)	//Sobel��Ե���
	{
		int Mask[18] = { -1, -2, -1, 0, 0, 0, 1, 2, 1, -1, 0, 1, -2, 0, 2, -1, 0, 1 };
		ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 1, true);
	}
	else if (type == EDGEPREWITT)	//Prewitt��Ե���
	{
		int Mask[18] = { -1, -1, -1, 0, 0, 0, 1, 1, 1, 1, 0, -1, 1, 0, -1, 1, 0, -1 };
		ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 1, true);
	}
	else if (type == EDGEROBERTS)	//Roberts��Ե���
	{
		int Mask[18] = { 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, -1, 0, 0, 0, 0, 0 };
		ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 1, true);
	}
}

//ͳ�������˲�����
void OrderStaticFilterProcessing(char *oImage, char *nImage, int wImage, int hImage, int MaskWH, int type)	//MaskWH - ������Χ
{
	int i, j, m, n, k;
	vector<int> sample;
	k = (MaskWH - 1) / 2;
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			sample.clear();
			for (m = -k; m <= k; m++)
			{
				for (n = -k; n <= k; n++)
				{
					sample.push_back((BYTE)oImage[(i + m)*wImage + (j + n)]);
				}
			}
			if (type == MAXFILTER)
				nImage[i*wImage + j] = (unsigned char)(*max_element(sample.begin(), sample.end()));
			else if (type == MINFILTER)
				nImage[i*wImage + j] = (unsigned char)(*min_element(sample.begin(), sample.end()));
			else if (type == MIDFILTER)
			{
				sort(sample.begin(), sample.end());
				nImage[i*wImage + j] = (unsigned char)(sample[4]);
			}
		}
	}
}

//ͼ��򵥼��α任����
void SimpleGeometricTranslation(char *oImage, char *nImage, int wImage, int hImage, double *sgt_Array)
{
	double axy, bxy;		int i, sx, sy, ox, oy;
	for (i = 0; i < wImage*hImage; i++) nImage[i] = 0;		  //�����ͼ��
	for (sy = 0; sy < hImage; sy++) {
		for (sx = 0; sx < wImage; sx++) {
			ox = sx - wImage / 2;				  //��ͼ������Ϊԭ��
			oy = sy - hImage / 2;
			axy = sgt_Array[0] * (double)ox + sgt_Array[1] * (double)oy + sgt_Array[2];
			bxy = sgt_Array[3] * (double)ox + sgt_Array[4] * (double)oy + sgt_Array[5];
			axy += wImage / 2;				  //�ָ�ͼ��ԭ��λ��
			bxy += hImage / 2;
			if ((axy < 0.0) || (axy >= wImage - 1) || (bxy < 0.0) || (bxy >= hImage - 1))
				nImage[sy*wImage + sx] = 0;			  //������Χ������0
			else
				nImage[sy*wImage + sx] = oImage[((int)bxy)*wImage + (int)axy];
		}
	}
}

//ͼ��ƽ�Ʊ任����
void ImageTranslation(char *oImage, char *nImage, int wImage, int hImage, int xPos, int yPos)
{
	double gt_Array[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

	gt_Array[2] = xPos;				 //ˮƽ����ƽ�ƾ���
	gt_Array[5] = yPos;				 //��ֱ����ƽ�ƾ���

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}

//ͼ�����ű任����
void ImageScaling(char *oImage, char *nImage, int wImage, int hImage, double xScaling, double yScaling)
{
	double gt_Array[9] = { 0, 0, 0,0, 0, 0,0, 0, 1 };

	gt_Array[0] = 1.0 / xScaling;		 //ˮƽ����Ŵ���
	gt_Array[4] = 1.0 / yScaling;		 //��ֱ����Ŵ���

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}

//ͼ����ת�任����
void ImageRotation(char *oImage, char *nImage, int wImage, int hImage, double iAngle)
{
	double pi = 3.14159;
	double gt_Array[9] = { 0, 0, 0,
		0, 0, 0,
		0, 0, 1 };
	iAngle = (iAngle / 360) * 2 * pi;
	gt_Array[0] = cos(iAngle);
	gt_Array[1] = -sin(iAngle);
	gt_Array[3] = sin(iAngle);
	gt_Array[4] = cos(iAngle);

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}
