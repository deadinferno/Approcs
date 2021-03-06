#pragma once
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <string>

using namespace std;

/* Рисовать график */
void drawGraphic(HDC hDC, HWND hWnd, vector<double> xX, vector<double> yY, COLORREF NewColor);

/* Рисовать полином */
void drawPolynom(HDC hDC, HWND hWnd, vector<double> xX, vector<double> yY);

/* Рисовать систему координат */
void drawCoordinateLines(HDC hDC, HWND hWnd);