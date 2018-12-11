#include "drawing.h"
#include "approcs.h"

struct POINTXY
{
	double x;
	double y;

	POINTXY() {};

	POINTXY(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
};

struct COORDSYSTEM
{
	POINTXY vertTop;
	POINTXY vertBottom;
	POINTXY horzLeft;
	POINTXY horzRight;
	POINTXY startPoint;
	double XStep;
	double YStep;

	COORDSYSTEM() {};

	COORDSYSTEM(
		POINTXY vertTop,
		POINTXY vertBottom,
		POINTXY horzLeft,
		POINTXY horzRight,
		POINTXY startPoint,
		double XStep,
		double YStep)
	{
		this->vertTop = vertTop;
		this->vertBottom = vertBottom;
		this->horzLeft = horzLeft;
		this->horzRight = horzRight;
		this->startPoint = startPoint;
		this->XStep = XStep;
		this->YStep = YStep;
	}

	void setPoints(
		POINTXY vertTop,
		POINTXY vertBottom,
		POINTXY horzLeft,
		POINTXY horzRight,
		POINTXY startPoint,
		double XStep,
		double YStep)
	{
		this->vertTop = vertTop;
		this->vertBottom = vertBottom;
		this->horzLeft = horzLeft;
		this->horzRight = horzRight;
		this->startPoint = startPoint;
		this->XStep = XStep;
		this->YStep = YStep;
	}

};

COORDSYSTEM System;

void drawPolynom(HDC hDC, HWND hWnd, vector<double> xX, vector<double> yY)
{
	RECT r;
	GetClientRect(hWnd, &r);

	vector<double> X = xX;
	vector<double> Y = yY;

	vector<double> aX;
	vector<double> aY;

	double STEP = 0.1;
	double newX = -10;

	// Заполняем новые иксы промежуточными значениями
	for (int i = 0; i < 500; i++)
	{
		aX.push_back(newX);
		aY.push_back(Lagranz(X, Y, aX[i]));
		newX += STEP;
	}

	drawGraphic(hDC, hWnd, aX, aY, RGB(255, 0, 0));

	// Заполняем новые игреки

	/*
	
		X, Y - иксы и игреки многочлена
		pointX - точка, для которой нужно узнать значение полинома
		метод возвращает значение полинома в указанной точке, т.е. возвращает Y

		string p = to_string(Lagranz(X, Y, pointX));
	
	*/
}

void drawGraphic(HDC hDC, HWND hWnd, vector<double> xX, vector<double> yY, COLORREF NewColor)
{
	RECT r;
	GetClientRect(hWnd, &r);

	vector<double> X = xX;
	vector<double> Y = yY;
	
	POINTXY _1stPoint, _2ndPoint;
	_1stPoint = System.startPoint; // Первой точкой рисования устанавливаем начало координат

	HPEN hPen = CreatePen(PS_SOLID, 1, NewColor);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	System.XStep *= 1;

	for (int i = 0; i < (X.size() - 1); i++)
	{
		_1stPoint.x = System.startPoint.x + (System.XStep * X[i]);
		_1stPoint.y = System.startPoint.y + ((System.YStep * (-1)) * Y[i]);

		_2ndPoint.x = System.startPoint.x + (System.XStep * (X[i + 1]));
		_2ndPoint.y = System.startPoint.y + ((System.YStep * (-1)) * (Y[i + 1]));

		// Помещаем точку рисования в начало координат
		MoveToEx(hDC, _1stPoint.x, _1stPoint.y, NULL);
		LineTo(hDC, _2ndPoint.x, _2ndPoint.y);
	}

	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void drawCoordinateLines(HDC hDC, HWND hWnd)
{
	RECT r;
	GetClientRect(hWnd, &r);

	double STEP = r.right / 40;

	POINTXY vL1stP(r.right / 4, r.bottom / 40); // vertical line 1st point
	POINTXY vL2ndP(r.right / 4, r.bottom - (r.bottom / 40)); // vertical line 2nd point
	POINTXY hL1stP(r.right / 40, r.bottom / 2); // horisontal line 1st point
	POINTXY hL2ndP(r.right - (r.right / 40), r.bottom / 2); // horisontal line 2nd point

	POINTXY XYStart(vL1stP.x, hL1stP.y);

	System.setPoints(vL1stP, vL2ndP, hL1stP, hL2ndP, XYStart, STEP, STEP);

	/****************************************************/
	/*			Рисуем координатную сетку				*/
	/****************************************************/


	

	RECT rNew;

	COLORREF NewColor = RGB(220, 220, 200);
	HPEN hPen = CreatePen(PS_SOLID, 1, NewColor);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	// Вертинальные линии
	rNew = r;

	for (int i = 0; i <= (r.right / STEP); i++)
	{
		MoveToEx(hDC, rNew.left, rNew.top, NULL);
		LineTo(hDC, rNew.left, rNew.bottom);

		rNew.left += STEP;
	}

	// Горизонтальные линии
	rNew = r;

	for (int i = 0; i <= (r.bottom / STEP); i++)
	{
		MoveToEx(hDC, rNew.left, rNew.top, NULL);
		LineTo(hDC, rNew.right, rNew.top);

		rNew.top += STEP;
	}

	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	/****************************************************/
	/*			Рисуем координатные линии				*/
	/****************************************************/

	// Рисуем вертикальную линию координат
	MoveToEx(hDC, vL1stP.x, vL1stP.y, NULL);
	LineTo(hDC, vL2ndP.x, vL2ndP.y);

	// Рисуем горизонтальную линию координат
	MoveToEx(hDC, hL1stP.x, hL1stP.y, NULL);
	LineTo(hDC, hL2ndP.x, hL2ndP.y);



	/****************************************************/
	/*			Рисуем шапки линиям						*/
	/****************************************************/

	/*			Для вертикальной			 */
	MoveToEx(hDC, vL1stP.x, vL1stP.y, NULL);
	LineTo(hDC, vL1stP.x - (STEP / 2), vL1stP.y + (STEP / 2));

	MoveToEx(hDC, vL1stP.x, vL1stP.y, NULL);
	LineTo(hDC, vL1stP.x + (STEP / 2), vL1stP.y + (STEP / 2));

	/*			Для горизонтальной			 */
	MoveToEx(hDC, hL2ndP.x, hL2ndP.y, NULL);
	LineTo(hDC, hL2ndP.x - (STEP / 2), hL2ndP.y - (STEP / 2));

	MoveToEx(hDC, hL2ndP.x, hL2ndP.y, NULL);
	LineTo(hDC, hL2ndP.x - (STEP / 2), hL2ndP.y + (STEP / 2));


	/****************************************************/
	/*			Рисуем деления линиям					*/
	/****************************************************/

	// Горизонтальной прямой
	POINTXY newP = hL1stP;
	newP.x += STEP; newP.y -= (STEP * (3.0 / 10));

	for (int i = 0; i + 1 < (hL2ndP.x - hL1stP.x) / STEP; i++)
	{
		MoveToEx(hDC, newP.x, newP.y, NULL);
		LineTo(hDC, newP.x, newP.y + (STEP * (7.0 / 10)));
		newP.x += STEP; /*newP.y -= 6*/;
	}


	// Вертикальной прямой 
	newP = vL1stP;
	newP.x -= (STEP * (3.0 / 10)); newP.y += STEP;

	for (int i = 0; i < (vL2ndP.y - vL1stP.y) / STEP; i++)
	{
		MoveToEx(hDC, newP.x, newP.y, NULL);
		LineTo(hDC, newP.x + (STEP * (7.0 / 10)), newP.y);
		newP.y += STEP; /*newP.y -= 6*/;
	}


	/****************************************************/
	/*			Рисуем подписи делениям					*/
	/****************************************************/

	HFONT font;

	font = CreateFont(
		STEP * 0.8,                         // Высота шpифта (в логических единицах).
		0,							// Шиpина шpифта (в логических единицах).
		0,							// Угол наклона стpоки (в десятых долях гpадуса).
		0,                          // Угол наклона оси символов (в десятых долях гpадуса).
		FW_NORMAL,                  // Вес шpифта (0 - 000).
		FALSE,                      // Шpифт - куpсив.
		FALSE,                      // Шpифт - с подчеpкиванием.
		FALSE,                      // Шpифт с пеpечеpкиванием.
		ANSI_CHARSET,               // Одна из констант ANSI_CharSet, OEM_CharSet, Symbol_CharSet.
		OUT_DEFAULT_PRECIS,         // Одна из констант Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
		CLIP_DEFAULT_PRECIS,        // Одна из констант Out_Character_Precis, Out_Default_Precis, Out_String_Precis, Out_Stroke_Precis.
		DEFAULT_QUALITY,            // Одна из констант Default_Quality, Draft_Quality, Proof_Quality.
		DEFAULT_PITCH | FF_ROMAN,   // Одна из констант Default_Pitch, Fixed_Pitch или Variable_Pitch, скомбиниpованная с одной из констант ff_Decorative, ff_DontCare, ff_Modern, ff_Roman, ff_Script или ff_Swiss.
		TEXT("Times New Roman Cyr")     // Название шpифта.
	);
	SelectObject(hDC, font);


	// Горизонтальной
	//MoveToEx(hDC, System.startPoint.x + STEP - , System.startPoint.y + STEP, NULL);
	
	POINTXY startPoint, lastPoint;
	startPoint.x = System.startPoint.x + (STEP - (STEP / 7.5));
	startPoint.y = System.startPoint.y + (STEP / 3.0);

	SetTextColor(hDC, RGB(120, 120, 120));

	for (int i = 0; i + 1 < ((System.horzRight.x - System.vertTop.x) / STEP); i++)
	{
		string iStr = to_string(i + 1);
		TextOut(hDC, startPoint.x + (STEP * i), startPoint.y, iStr.c_str(), strlen(iStr.c_str()));
	}

	// Вертикальной

	startPoint.x = System.startPoint.x - (STEP * 1.2 /*- (STEP / 15.5)*/);
	startPoint.y = System.startPoint.y - (STEP * 1.4);

	for (int i = 0; i + 1 < ((System.horzLeft.y - System.vertTop.y) / STEP); i++)
	{
		string iStr = to_string(i + 1);
		TextOut(hDC, startPoint.x, startPoint.y - (STEP * i), iStr.c_str(), strlen(iStr.c_str()));
	}

}
