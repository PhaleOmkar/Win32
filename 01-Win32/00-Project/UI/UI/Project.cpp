// Headers
#include <Windows.h>
#include <stdio.h>
#include "Project.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

// External functions
typedef double(*lpfnCalculateCentrifugalForce) (double, double, double);

// WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("Project");

	// code
	// initialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// register above class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szClassName,
		TEXT("Project"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient;
	static HBITMAP hbmpImage = NULL;

	HINSTANCE hInstance = NULL;
	HDC hdc, hdcCompatible;
	BITMAP bmpImage;
	PAINTSTRUCT ps;
	RECT rc;
	HFONT hFont = NULL;

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		hbmpImage = LoadBitmap(hInstance, MAKEINTRESOURCE(BMPEARTH));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'P':
			DialogBox(hInstance, TEXT("DLGBOX"), hwnd, DlgProc);
			break;

		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hdcCompatible = CreateCompatibleDC(hdc);
		GetObject(hbmpImage, sizeof(BITMAP), &bmpImage);
		SelectObject(hdcCompatible, hbmpImage);
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcCompatible, 0, 0, SRCCOPY);
		DeleteDC(hdcCompatible);

		GetClientRect(hwnd, &rc);
		hFont = CreateFont(30, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Consolas"));
		SelectObject(hdc, hFont);
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		DrawText(hdc, TEXT("Press P for Physics"), -1, &rc, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		DeleteObject(hFont);
		DeleteObject(hbmpImage);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HMODULE hDll = NULL;
	static lpfnCalculateCentrifugalForce CalculateCentrifugalForce = NULL;
	char szTmp[255] = "";
	double dMass = 0.0;
	double dVelocity = 0.0;
	double dRadius = 0.0;
	double dResult = 0.0;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		// Load Physics DLL
		hDll = LoadLibrary(TEXT("Physics.dll"));
		if (hDll == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to Physics DLL"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		CalculateCentrifugalForce = (lpfnCalculateCentrifugalForce)GetProcAddress(hDll, "CalculateCentrifugalForce");
		if (CalculateCentrifugalForce == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to locate CalculateCentrifugalForce function"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, ID_ETMASS, szTmp, 100);
			dMass = atof(szTmp);
			if (dMass == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect mass!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			GetDlgItemText(hDlg, ID_ETVELOCITY, szTmp, 100);
			dVelocity = atof(szTmp);
			if (dVelocity == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect velocity!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			GetDlgItemText(hDlg, ID_ETRADIUS, szTmp, 100);
			dRadius = atof(szTmp);
			if (dRadius == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect radius!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			dResult = CalculateCentrifugalForce(dMass, dVelocity, dRadius);

			sprintf_s(szTmp, TEXT("Mass: %g kg\nVelocity: %g m/s\nRadius: %g m\n\nCentrifugal Force: %g N"), dMass, dVelocity, dRadius, dResult);
			MessageBox(hDlg, TEXT(szTmp), TEXT("Message"), MB_OK | MB_ICONINFORMATION);
			sprintf_s(szTmp, "%g", dResult);
			SetDlgItemText(hDlg, ID_RESULT, szTmp);
			return TRUE;

		case IDCANCEL:
			FreeLibrary(hDll);
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
