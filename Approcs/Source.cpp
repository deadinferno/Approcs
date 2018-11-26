#include <windows.h>
#include <iostream>
#include <string> 
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <vector>
#include "resource1.h"
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | \
            WS_SYSMENU )


#define ID_EDIT_1 4001
#define ID_EDIT_2 4002

#define ID_EDIT_COUNT			1
#define ID_EDIT_CHECK		2

#define ID_BUTTON_LAGRANZ		4
#define ID_BUTTON_NEWTON		5
#define ID_BUTTON_BACK		6
#define ID_BUTTON_GRAPH		7
#define ID_BUTTON_EXAMP		8
#define ID_BUTTON_EXERC		9
#define ID_BUTTON_SET_COUNT		10
#define ID_BUTTON_DRAW		11
#define ID_BUTTON_CHECK		12

#define pi 3.14

#define HOME 0
#define LAGR 1
#define NEWT 2
using namespace std;
const int N = 10;



// --- �������� ������� �������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);
BOOL drawGraphic(HDC hDC);
BOOL drawHome(HDC hDC, HWND hWnd);
BOOL drawInfo(HDC hDC, HWND hWnd, int method);
BOOL drawGraph(HDC hDC, HWND hWnd, int method);
BOOL drawExample(HDC hDC, HWND hWnd, int method);
BOOL drawExercises(HDC hDC, HWND hWnd, int method);
VOID reset();
VOID resetTable();


int valCount = 5;
int oldCount;

// --- ���������� ����������
HWND hBtnLagr;
HWND hBtnNewt;
HWND hEditMask;
HWND hBackBtn, hGraphBtn, hExampBtn, hExercBtn, hCountBtn, hDrawBtn, hCheckBtn;
HWND hEditDir;
HWND hWndChild;		// ���������� ��������� ���� ����������
HINSTANCE hInst; 		// ���������� ���������� ����������
char ClassName[] = "Window"; 		// �������� ������ ����
char AppTitle[] = "Application Win32"; 	// ��������� �������� ����
HFONT fontBig;
HFONT font25;
HFONT font20;


HBITMAP hBitmapLagrExamp = NULL;
HBITMAP hBitmapNewtExamp = NULL;
HBITMAP hBitmapLagrInfo = NULL;
HBITMAP hBitmapNewtInfo = NULL;

HBITMAP hBitmapLagrExerc1 = NULL;
HBITMAP hBitmapLagrExerc1_ans = NULL;
HBITMAP hBitmapLagrExerc2 = NULL;
HBITMAP hBitmapLagrExerc2_ans = NULL;

HBITMAP hBitmapNewtExerc1 = NULL;
HBITMAP hBitmapNewtExerc1_ans = NULL;

HWND hEditCount, hEditCheck;					// ����������� textBox(��)
HWND **table;


HWND hMainWin;




enum screen {info, graphic, examples, exercises};

int curScreen = info;
int curMethod = HOME;

bool inWork = false;
bool runTable = false;
bool runGraph = false;
bool result = false;
vector<double> X;
vector<double> Y;

double pointX;

double Lagranz(vector< double > X, vector< double > Y, double t) {
	double z, p1, p2;
	z = 0;
	int n = X.size();
	for (int j = 0; j<n; j++) {
		p1 = 1; p2 = 1;
		for (int i = 0; i<n; i++) {
			if (i == j) {
				p1 = p1 * 1; p2 = p2 * 1;
			}
			else {
				p1 = p1*(t - X[i]);
				p2 = p2*(X[j] - X[i]);
			}
		}
		z = z + Y[j] * p1 / p2;
	}
	return z;
}

double Newton(vector< double > X, vector< double > Y, double x) {
	double res = Y[0], F, den;
	int i, j, k;
	int n = X.size();
	for (i = 1; i<n; i++) {
		F = 0;
		for (j = 0; j <= i; j++) {//��������� ��������� ��������
			den = 1;
			//������� ����������� ����������� ��������
			for (k = 0; k <= i; k++) {
				if (k != j) den *= (X[j] - X[k]);
			}
			//������� ����������� ��������
			F += Y[j] / den;
		}
		//��������� ����������� �������� �� ������ (x-x[0])...(x-x[i-1])
		for (k = 0; k<i; k++)F *= (x - X[k]);
		res += F;//�������
	}
	return res;
}


// --- ������� WinMain
int WINAPI WinMain(HINSTANCE hInstance,
	// ���������� ���������� ����������
	HINSTANCE hPrevInstance, // � Win32 ������ ����� NULL
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	setlocale(LC_ALL, "Russian");
	WNDCLASS wc; 	// ��������� ��� ���������� � ������ ����
	HWND hWnd; 	// ���������� �������� ���� ����������
	MSG msg; 	// ��������� ��� �������� ���������
				// ��������� ���������� ���������� ���������� � ����������
				// ����������, ����� ��� ������������� ��������������� �� �
				// ������� ����.
	hInst = hInstance;

	if ((hWnd = FindWindow(ClassName, NULL)) != NULL)
	{

		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);

		// ������� ���������� ����� - ������ ����� �����
		// ������������.
		return FALSE;
	}

	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;		// ��� ������ ����
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// ����� ������� �������
	wc.style = CS_HREDRAW | CS_VREDRAW;	// ����� ������ 
	wc.hInstance = hInstance;		// ��������� ����������
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	// ����������� ��� ����
	wc.hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));//��� �������������� ������ ����� � ��������� �� ������ ������� ���������� ������
																 // ������ ���� ��� ����
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// ����� ��� ����
	wc.lpszMenuName = NULL;			// ������ ���� ����
	wc.cbClsExtra = 0;			// �������������� ������
	wc.cbWndExtra = 0;			// �������������� ������
								// P���������� ������ ����.
	RegisterClass(&wc);




	char title[128];
	LoadStringA(hInst, IDS_STRING1, title, sizeof(title));

																			  // ������� ������� ���� ����������.
	hWnd = CreateWindow(
		ClassName, 			// ��� ������ ����
		"���������������� ����������",			// ��������� ���� 
		WS_OVERLAPPEDWINDOW, 		// ����� ����
		CW_USEDEFAULT,			// X-���������� 
		CW_USEDEFAULT,			// Y-���������� 
		/*CW_USEDEFAULT*/1000,			// ������ ����
		/*CW_USEDEFAULT*/800,			// ������ ����
		NULL,			// ���������� ����-��������
		NULL,			// ���������� ���� ����
		hInst,		// ���������� ���������� ����������
		NULL);		// �������������� ����������
	if (!hWnd)
	{
		// ���� �� �������, ������ ��������������.
		MessageBox(NULL,
			"Create: error", AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// ���������� ����.
	ShowWindow(hWnd, nCmdShow);

	// ��������� ���������� ���������� ������� ����.
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


int i, xView, yView;
double y;
char Buf[2];

// --- ������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

	switch (msg)
	{
	case WM_CREATE:
	{	
		hBitmapLagrExamp = (HBITMAP)LoadImage(hInst, ".\\LagrExamp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitmapNewtExamp = (HBITMAP)LoadImage(hInst, ".\\NewtExamp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		hBitmapLagrInfo = (HBITMAP)LoadImage(hInst, ".\\LagrInfo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitmapNewtInfo = (HBITMAP)LoadImage(hInst, ".\\NewtInfo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		hBitmapLagrExerc1 = (HBITMAP)LoadImage(hInst, ".\\LagrExerc1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitmapLagrExerc1_ans = (HBITMAP)LoadImage(hInst, ".\\LagrExerc1_ans.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitmapLagrExerc2 = (HBITMAP)LoadImage(hInst, ".\\LagrExerc2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitmapLagrExerc2_ans = (HBITMAP)LoadImage(hInst, ".\\LagrExerc2_ans.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		hBitmapNewtExerc1 = (HBITMAP)LoadImage(hInst, ".\\NewtExerc1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitmapNewtExerc1_ans = (HBITMAP)LoadImage(hInst, ".\\NewtExerc1_ans.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	}; break;
	case WM_SIZE:
	{
		xView = LOWORD(lParam);
		yView = HIWORD(lParam);

		break;
	}
		
	// ���������� �������� ���������� ���������� ������� ����.
	case WM_PAINT:
	{
		if (runTable)
		{
			resetTable();
		}
		reset();
		
		HDC hDC;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		// �������� �������� ����


		if (curMethod == HOME)
		{
			drawHome(hDC, hWnd);
		}
		else
		{
			switch (curScreen)
			{
			case info:
			{
				drawInfo(hDC, hWnd, curMethod);
				//inWork = true;
				break;
			};
			case graphic:
			{
				//inWork = true;
				drawGraph(hDC, hWnd, curMethod);
				
				break;
			};
			case examples:
			{
				drawExample(hDC, hWnd, curMethod);
				break;
			};
			case exercises:
			{
				drawExercises(hDC, hWnd, curMethod);
				break;
			};
			}
		}

		EndPaint(hWnd, &ps); // ���������� �������� ����

	}; break;

	case WM_COMMAND:
	{
		switch (wParam)
		{
			// �����
		case ID_BUTTON_NEWTON:
		{
			curScreen = info;
			curMethod = NEWT;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		};	break;


		case ID_BUTTON_BACK:
		{
			curScreen = info;
			curMethod = HOME;
			inWork = false;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		};	break;

		case ID_BUTTON_LAGRANZ:
		{
			curScreen = info;
			curMethod = LAGR;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}; break;

		case ID_BUTTON_GRAPH:
		{
			curScreen = graphic;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}; break;

		case ID_BUTTON_EXAMP:
		{
			curScreen = examples;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}; break;

		case ID_BUTTON_EXERC:
		{
			curScreen = exercises;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}; break;

		case ID_BUTTON_CHECK:
		{
			char out_str[16];
			GetWindowText(hEditCheck, out_str, 16);
			if (atoi(out_str) && (atoi(out_str) > 0) && (atoi(out_str) < 15))
			{
				pointX = atoi(out_str);
				result = true;
			}
			else
			{
				MessageBox(NULL,
					"������������ ��������!", "������ �����", MB_OK | MB_ICONSTOP);
			}
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}; break;

		case ID_BUTTON_SET_COUNT:
		{
			char out_str[16];
			GetWindowText(hEditCount, out_str, 16);
			if (atoi(out_str) && (atoi(out_str) > 0) && (atoi(out_str) < 12))
			{
				oldCount = valCount;
				valCount = atoi(out_str);
				inWork = true;
				
			}
			else
			{
				MessageBox(NULL,
					"������������ ��������!", "������ �����", MB_OK | MB_ICONSTOP);
			}
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}; break;

		case ID_BUTTON_DRAW:
		{
			char out_str[16];
			bool bad = false;
			X.clear();
			Y.clear();
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < valCount; j++)
				{
					char out_str[16];
					GetWindowText(table[i][j], out_str, 16);
					if (atoi(out_str) && (i == 0))
					{
						X.push_back(atoi(out_str));
					}
					else if (atoi(out_str) && (i == 1))
					{
						Y.push_back(atoi(out_str));
					}
					else
					{
						MessageBox(NULL,
							"������������ ��������!", "������ �����", MB_OK | MB_ICONSTOP);
						bad = true;
						break;
					}
				}
				if (bad)
				{
					break;
				}
			}

			if (!bad)
			{
				runGraph = true;
			}

			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}; break;

		}
	}; break;

	// ������������ ������ ����.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}; break;

	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}

//������� ��������� �����
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //������� �������� ���������� x1, y1
	return LineTo(hdc, x2, y2); //���������� �����
}

double VecMax(vector<double> vec)
{
	double max = vec[0];
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] >= max) max = vec[i];
	}
	return max;
}
double VecMin(vector<double> vec)
{
	double min = vec[0];
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] <= min) min = vec[i];
	}
	return min;
}

BOOL drawGraphic(HDC hDC)
{
	double maxX = VecMax(X);
	double minX = VecMin(X);
	double maxY = VecMax(Y);
	double minY = VecMin(Y);


	HPEN hPen; //������ ����
	
	SetMapMode(hDC, MM_ISOTROPIC); //���������� ������� ����������, ��� ����������
	SetWindowExtEx(hDC, 500, 500, NULL); //����� ����
	//SetWindowExtEx(hDC, abs(maxX) + 1 + abs(minX) + 1, abs(maxY) + 1 + abs(minY) + 1, NULL); //����� ����
	SetViewportExtEx(hDC, xView/2, -yView/2, NULL); //���������� ������ ������
	SetViewportOrgEx(hDC,240 + xView / 12, 60 + yView / 4, NULL); //������ ���������

													   //������ ��� ���������
	Line(hDC, 0, 220, 0, -220);//��� �
	Line(hDC, -100, 0, 500, 0);//��� �
	MoveToEx(hDC, 0, 0, NULL); //������������ � ������ ���������


	
							   //�������� �������� ����
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hDC, hPen);

	/*//���������
	for (i = 0; i<450; i++) {
		y = 180.0*(exp(-i*0.01))*sin(pi*i*(200.0 / 400.0) / 180.0);

		LineTo(hDC, i, (int)y);
	}

	//������ ���� ����� ������
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hDC, hPen);

	//������� �������
	for (i = -100; i<500; i += 100)
	{
		Line(hDC, i, 3, i, -3); //�� ��� �
		Line(hDC, -3, i, 3, i); //�� ��� Y
		itoa(i, Buf, 10);
		TextOut(hDC, i - 5, -5, Buf, strlen(Buf));
		TextOut(hDC, -5, i, Buf, strlen(Buf));
	}*/

	
	return true;
}

BOOL drawHome(HDC hDC, HWND hWnd)
{
	RECT r;
	GetClientRect(hWnd, &r);//�������� ���������� ���������� ������� ����. 
	hMainWin = hWnd;
	int xb = 10, yb = 10, wb = 250, hb = 20,
		x = 10, y = 100, w = 1000 - 30, h = 200;


	
	fontBig = CreateFont(
	48,                         // ������ �p���� (� ���������� ��������).
	0,                         // ��p��� �p���� (� ���������� ��������).
	0,                        // ���� ������� ��p��� (� ������� ����� �p�����).
	0,                          // ���� ������� ��� �������� (� ������� ����� �p�����).
	FW_NORMAL,                  // ��� �p���� (0 - 000).
	FALSE,                      // �p��� - ��p���.
	FALSE,                      // �p��� - � �����p��������.
	FALSE,                      // �p��� � ��p���p��������.
	ANSI_CHARSET,               // ���� �� �������� ANSI_CharSet, OEM_CharSet, Symbol_CharSet.
	OUT_DEFAULT_PRECIS,         // ���� �� �������� Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
	CLIP_DEFAULT_PRECIS,        // ���� �� �������� Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
	DEFAULT_QUALITY,            // ���� �� �������� Default_Quality, Draft_Quality, Proof_Quality.
	DEFAULT_PITCH | FF_ROMAN,   // ���� �� �������� Default_Pitch, Fixed_Pitch ��� Variable_Pitch, ��������p������� � ����� �� �������� ff_Decorative, ff_DontCare, ff_Modern, ff_Roman, ff_Script ��� ff_Swiss.
	TEXT("Times New Roman Cyr")     // �������� �p����.
	);
	font25 = CreateFont(
		25,                         // ������ �p���� (� ���������� ��������).
		0,                         // ��p��� �p���� (� ���������� ��������).
		0,                        // ���� ������� ��p��� (� ������� ����� �p�����).
		0,                          // ���� ������� ��� �������� (� ������� ����� �p�����).
		FW_NORMAL,                  // ��� �p���� (0 - 000).
		FALSE,                      // �p��� - ��p���.
		FALSE,                      // �p��� - � �����p��������.
		FALSE,                      // �p��� � ��p���p��������.
		ANSI_CHARSET,               // ���� �� �������� ANSI_CharSet, OEM_CharSet, Symbol_CharSet.
		OUT_DEFAULT_PRECIS,         // ���� �� �������� Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
		CLIP_DEFAULT_PRECIS,        // ���� �� �������� Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
		DEFAULT_QUALITY,            // ���� �� �������� Default_Quality, Draft_Quality, Proof_Quality.
		DEFAULT_PITCH | FF_ROMAN,   // ���� �� �������� Default_Pitch, Fixed_Pitch ��� Variable_Pitch, ��������p������� � ����� �� �������� ff_Decorative, ff_DontCare, ff_Modern, ff_Roman, ff_Script ��� ff_Swiss.
		TEXT("Times New Roman Cyr")     // �������� �p����.
	);
	font20 = CreateFont(
		20,                         // ������ �p���� (� ���������� ��������).
		0,                         // ��p��� �p���� (� ���������� ��������).
		0,                        // ���� ������� ��p��� (� ������� ����� �p�����).
		0,                          // ���� ������� ��� �������� (� ������� ����� �p�����).
		FW_NORMAL,                  // ��� �p���� (0 - 000).
		FALSE,                      // �p��� - ��p���.
		FALSE,                      // �p��� - � �����p��������.
		FALSE,                      // �p��� � ��p���p��������.
		ANSI_CHARSET,               // ���� �� �������� ANSI_CharSet, OEM_CharSet, Symbol_CharSet.
		OUT_DEFAULT_PRECIS,         // ���� �� �������� Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
		CLIP_DEFAULT_PRECIS,        // ���� �� �������� Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
		DEFAULT_QUALITY,            // ���� �� �������� Default_Quality, Draft_Quality, Proof_Quality.
		DEFAULT_PITCH | FF_ROMAN,   // ���� �� �������� Default_Pitch, Fixed_Pitch ��� Variable_Pitch, ��������p������� � ����� �� �������� ff_Decorative, ff_DontCare, ff_Modern, ff_Roman, ff_Script ��� ff_Swiss.
		TEXT("Times New Roman Cyr")     // �������� �p����.
	);
	SelectObject(hDC, fontBig);

	string Buf = "����� ���������� � ��������� ���������";
	TextOut(hDC, 100, 60, Buf.c_str(), strlen(Buf.c_str()));

	SelectObject(hDC, font25);
	Buf = "����: ������ ������������� �������. ���������� ������� � ��������";
	TextOut(hDC, 120, 120, Buf.c_str(), strlen(Buf.c_str()));

	r.top = 180;
	r.left = 200;
	r.bottom = 260;
	r.right = 800;
	SelectObject(hDC, font20);
	Buf = "    ������������, ���������������� (�� ���. inter�polis � \"������������, �����������, ����������; ���������������\") � � �������������� ���������� ������ ���������� ������������� �������� �������� �� ���������� ����������� ������ ��������� ��������.";
	DrawText(hDC, Buf.c_str(), Buf.length(), &r, DT_WORDBREAK);
	r.top = 270;
	r.left = 200;
	r.bottom = 350;
	r.right = 800;
	Buf = "    � ������ ��������� ������������ ���������� � ���� ������� ������������: � ������� �������� ������� � �������� ��������. ������� �� ��������������� ������, ����� ������������ � ���� ���������.";
	DrawText(hDC, Buf.c_str(), Buf.length(), &r, DT_WORDBREAK);

	hBtnLagr = CreateWindow("BUTTON", "���������������� ������� ��������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		200, 420, 280, 120,
		hMainWin, (HMENU)ID_BUTTON_LAGRANZ, hInst, NULL);
	hBtnNewt = CreateWindow("BUTTON", "���������������� ������� �������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		520, 420, 280, 120,
		hMainWin, (HMENU)ID_BUTTON_NEWTON, hInst, NULL);
	return true;
}

BOOL drawInfo(HDC hDC, HWND hWnd, int method)
{


	hGraphBtn = CreateWindow("BUTTON", "���������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 40, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_GRAPH, hInst, NULL);
	hExampBtn = CreateWindow("BUTTON", "������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 90, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXAMP, hInst, NULL);
	hExercBtn = CreateWindow("BUTTON", "����������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 140, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXERC, hInst, NULL);
	hBackBtn = CreateWindow("BUTTON", "�����", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 680, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_BACK, hInst, NULL);

	if (method == LAGR)
	{
		SelectObject(hDC, fontBig);

		string Buf = "���������������� ������� ��������";
		TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapLagrInfo);

		GetObject(hBitmapLagrInfo, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 210, 150, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
	}
	else if (method == NEWT)
	{
		SelectObject(hDC, fontBig);

		string Buf = "���������������� ������� �������";
		TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapNewtInfo);

		GetObject(hBitmapNewtInfo, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 210, 150, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
	}
	return true;
}

VOID reset()
{
	//inWork = false;
	runTable = false;
	oldCount = valCount;
	DestroyWindow((HWND)hBtnLagr);
	DestroyWindow((HWND)hBtnNewt);
	DestroyWindow((HWND)hGraphBtn);
	DestroyWindow((HWND)hBackBtn);
	DestroyWindow((HWND)hExercBtn);
	DestroyWindow((HWND)hExampBtn);
	DestroyWindow((HWND)hCountBtn);
	DestroyWindow((HWND)hDrawBtn);
	DestroyWindow((HWND)hEditCount);
	DestroyWindow((HWND)hCheckBtn);
	DestroyWindow((HWND)hEditCheck);


}

VOID resetTable()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < oldCount; j++)
		{
			DestroyWindow((HWND)table[i][j]);
		}
	}
	for (int i = 0; i<2; i++) delete[] table[i];
	delete[] table;
	
}

BOOL drawGraph(HDC hDC, HWND hWnd, int method)
{
	RECT r;
	GetClientRect(hWnd, &r);//�������� ���������� ���������� ������� ����. 


	hGraphBtn = CreateWindow("BUTTON", "���������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 40, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_GRAPH, hInst, NULL);
	hExampBtn = CreateWindow("BUTTON", "������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 90, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXAMP, hInst, NULL);
	hExercBtn = CreateWindow("BUTTON", "����������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 140, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXERC, hInst, NULL);
	hBackBtn = CreateWindow("BUTTON", "�����", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 680, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_BACK, hInst, NULL);



	hEditCount = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 320, 220, 60, 40, hMainWin, (HMENU)ID_EDIT_COUNT, hInst, NULL);
	hCountBtn = CreateWindow("BUTTON", "��", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		390, 220, 60, 40,
		hMainWin, (HMENU)ID_BUTTON_SET_COUNT, hInst, NULL);
	TextOut(hDC, 220, 230, "���-�� �����:", strlen("���-�� �����:"));
	

	

	if (runGraph)
	{
		TextOut(hDC, 220, 410, "X:", strlen("X:"));
		hEditCheck = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 240, 400, 60, 40, hMainWin, (HMENU)ID_EDIT_CHECK, hInst, NULL);
		hCheckBtn = CreateWindow("BUTTON", "���������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			320, 400, 100, 40,
			hMainWin, (HMENU)ID_BUTTON_CHECK, hInst, NULL);
	}
	

	if (inWork)
	{
		

		TextOut(hDC, 220, 290, "X:", strlen("X:"));
		TextOut(hDC, 220, 330, "Y:", strlen("Y:"));
		table = new HWND*[2];
		for (int i = 0; i < 2; i++)
		{
			table[i] = new HWND[valCount];
		}

		int tWidth = 40;
		int tHeight = 40;

		int xPos = 250;
		int yPos = 280;
		int tX;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < valCount; j++)
			{
				table[i][j] = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, xPos, yPos, tWidth, tHeight, hMainWin, NULL, hInst, NULL);
				xPos += tWidth;
			}
			tX = xPos;
			xPos = 250;
			yPos += tHeight;
		}
		runTable = true;

		hDrawBtn = CreateWindow("BUTTON", "���������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			tX + 10, 300, 90, 40,
			hMainWin, (HMENU)ID_BUTTON_DRAW, hInst, NULL);
	}
	


	if (method == LAGR)
	{
		
		SelectObject(hDC, fontBig);

		string Buf = "���������������� ������� ��������";
		TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		r.top = 120;
		r.left = 220;
		r.bottom = 200;
		r.right = 820;
		SelectObject(hDC, font20);
		

		Buf = "    ����� �� ������ ������ �������� ����������������� �������� �������� � ���������� �����. ��� ������ ������� ���������� �����, ������������ �������, ����� ������� ���������� � ������� �� ������ \"���������\". � ����������� ���� ������� �������� � � ������� ������ \"���������\".";
		DrawText(hDC, Buf.c_str(), Buf.length(), &r, DT_WORDBREAK);


		if (result)
		{
			TextOut(hDC, 220, 500, "������� �������� � ������ ����� �����:", strlen("������� �������� � ������ ����� �����:"));
			string p = to_string(Lagranz(X, Y, pointX));
			TextOut(hDC, 600, 500, p.c_str(), strlen(p.c_str()));
		}
	}
	else if (method == NEWT)
	{
		
		SelectObject(hDC, fontBig);

		string Buf = "���������������� ������� �������";
		TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		r.top = 120;
		r.left = 220;
		r.bottom = 200;
		r.right = 820;
		SelectObject(hDC, font20);


		Buf = "    ����� �� ������ ������ �������� ����������������� �������� ������� � ���������� �����. ��� ������ ������� ���������� �����, ������������ �������, ����� ������� ���������� � ������� �� ������ \"���������\". � ����������� ���� ������� �������� � � ������� ������ \"���������\".";
		DrawText(hDC, Buf.c_str(), Buf.length(), &r, DT_WORDBREAK);

		if (result)
		{
			TextOut(hDC, 220, 500, "������� ������� � ������ ����� �����:", strlen("������� ������� � ������ ����� �����:"));
			string p = to_string(Newton(X, Y, pointX));
			TextOut(hDC, 600, 500, p.c_str(), strlen(p.c_str()));
		}
	}
	return true;
}





BOOL drawExample(HDC hDC, HWND hWnd, int method)
{


	hGraphBtn = CreateWindow("BUTTON", "���������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 40, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_GRAPH, hInst, NULL);
	hExampBtn = CreateWindow("BUTTON", "������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 90, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXAMP, hInst, NULL);
	hExercBtn = CreateWindow("BUTTON", "����������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 140, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXERC, hInst, NULL);
	hBackBtn = CreateWindow("BUTTON", "�����", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 680, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_BACK, hInst, NULL);

	if (method == LAGR)
	{
		SelectObject(hDC, font25);

		string Buf = "���������������� ������� ��������. ������";
		//TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapLagrExamp);

		GetObject(hBitmapLagrExamp, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260, 15, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
		
	}
	else if (method == NEWT)
	{
		SelectObject(hDC, font25);

		string Buf = "���������������� ������� �������.  ������";
		//TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapNewtExamp);

		GetObject(hBitmapNewtExamp, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260, 15, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
	}
	
	return true;
}

BOOL drawExercises(HDC hDC, HWND hWnd, int method)
{


	hGraphBtn = CreateWindow("BUTTON", "���������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 40, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_GRAPH, hInst, NULL);
	hExampBtn = CreateWindow("BUTTON", "������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 90, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXAMP, hInst, NULL);
	hExercBtn = CreateWindow("BUTTON", "����������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 140, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_EXERC, hInst, NULL);
	hBackBtn = CreateWindow("BUTTON", "�����", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 680, 160, 40,
		hMainWin, (HMENU)ID_BUTTON_BACK, hInst, NULL);

	if (method == LAGR)
	{
		SelectObject(hDC, font25);

		string Buf = "���������������� ������� ��������. ������";
		//TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;
		int prevHeight = 0;
		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapLagrExerc1);

		GetObject(hBitmapLagrExerc1, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260, 30 + prevHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		prevHeight += bitmap.bmHeight;
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapLagrExerc1_ans);

		GetObject(hBitmapLagrExerc1_ans, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260 , 30 + prevHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		prevHeight += bitmap.bmHeight;
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapLagrExerc2);

		GetObject(hBitmapLagrExerc2, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260, 30 + prevHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		prevHeight += bitmap.bmHeight;
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapLagrExerc2_ans);

		GetObject(hBitmapLagrExerc2_ans, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260, 30 + prevHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		prevHeight += bitmap.bmHeight;
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

	}
	else if (method == NEWT)
	{
		SelectObject(hDC, font25);

		string Buf = "���������������� ������� �������.  ������";
		//TextOut(hDC, 210, 40, Buf.c_str(), strlen(Buf.c_str()));

		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;
		int prevHeight = 0;
		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapNewtExerc1);

		GetObject(hBitmapNewtExerc1, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260, 130 + prevHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		prevHeight += bitmap.bmHeight;
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		hdcMem = CreateCompatibleDC(hDC);
		oldBitmap = SelectObject(hdcMem, hBitmapNewtExerc1_ans);

		GetObject(hBitmapNewtExerc1_ans, sizeof(bitmap), &bitmap);
		BitBlt(hDC, 260, 130 + prevHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		prevHeight += bitmap.bmHeight;
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
	}

	return true;
}