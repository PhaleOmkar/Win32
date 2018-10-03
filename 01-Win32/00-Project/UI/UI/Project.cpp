// Headers
#include <Windows.h>
#include <stdio.h>
#include "Project.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

void Disable(HWND, int);
void Reset(HWND);

// External functions
typedef double(*lpfnCalculateCentripetalForce) (double, double, double);
typedef double(*lpfnCalculateCentripetalAcceleration) (double, double);

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
			DialogBox(hInstance, TEXT("DLGPHY"), hwnd, DlgProc);
			break;
		
		case 'C':
			DialogBox(hInstance, TEXT("DLGCHEM"), hwnd, DlgProc);
			break;
		
		case 'M':
			DialogBox(hInstance, TEXT("DLGMATHS"), hwnd, DlgProc);
			break;
		
		case 'B':
			DialogBox(hInstance, TEXT("DLGBIO"), hwnd, DlgProc);
			break;

		case 'Q':
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
		SetBkMode(hdc, TRANSPARENT);
		
		SetTextColor(hdc, RGB(255, 255, 255));
		DrawText(hdc, TEXT("\n\n\n\n\n        Press    for Physics      "), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n            Press    for Chemistry"), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n        Press    for Mathematics  "), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n            Press    for Biology  "), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n        Press    to Quit      "), -1, &rc, DT_VCENTER | DT_LEFT);

		SetTextColor(hdc, RGB(255, 0, 0));
		DrawText(hdc, TEXT("\n\n\n\n\n              P                   "), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n                  C               "), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n              M                   "), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n                  B               "), -1, &rc, DT_VCENTER | DT_LEFT);
		DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n              Q               "), -1, &rc, DT_VCENTER | DT_LEFT);

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
	static HANDLE hFile = NULL;
	static int iNoOfFile = 0;
	static int iForce = 0;
	static lpfnCalculateCentripetalForce CalculateCentripetalForce = NULL;
	static lpfnCalculateCentripetalAcceleration CalculateCentripetalAcceleration = NULL;
	static double dOldMass;
	static double dOldVelocity;
	static double dOldRadius;
	double dMass;
	double dVelocity;
	double dRadius;
	char szTmp[255] = "";
	double dResult = 0.0;
	HDC hDC = NULL;
	HWND hControl = NULL;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		// initialize variable
		dOldMass = 0.0;
		dOldVelocity = 0.0;
		dOldRadius = 0.0;
		dMass = 0.0;
		dVelocity = 0.0;

		// initialize radio button
		iForce = ID_RBCPFORCE;
		CheckRadioButton(hDlg, ID_RBCPFORCE, ID_RBCPACC, iForce);
		Disable(hDlg, iForce);

		// initialize close button
		hControl = GetDlgItem(hDlg, ID_CLOSE);
		hDC = GetDC(hControl);
		SetBkColor(hDC, RGB(255, 0, 0));
		UpdateWindow(hControl);
		ReleaseDC(hControl, hDC);

		// Load Physics DLL
		hDll = LoadLibrary(TEXT("Physics.dll"));
		if (hDll == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to Physics DLL"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		CalculateCentripetalForce = (lpfnCalculateCentripetalForce)GetProcAddress(hDll, "CalculateCentripetalForce");
		if (CalculateCentripetalForce == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to locate CalculateCentripetalForce function"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		CalculateCentripetalAcceleration = (lpfnCalculateCentripetalAcceleration)GetProcAddress(hDll, "CalculateCentripetalAcceleration");
		if (CalculateCentripetalAcceleration == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to locate CalculateCentrifugalForce function"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case ID_RBCPFORCE:
		case ID_RBCPACC:
			iForce = LOWORD(wParam);
			Disable(hDlg, iForce);
			return TRUE;

		case ID_CP_COMPUTE:
			GetDlgItemText(hDlg, ID_CP_ETMASS, szTmp, 100);
			dMass = atof(szTmp);
			if (dMass == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect mass!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			GetDlgItemText(hDlg, ID_CP_ETVELOCITY, szTmp, 100);
			dVelocity = atof(szTmp);
			if (dVelocity == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect velocity!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			GetDlgItemText(hDlg, ID_CP_ETRADIUS, szTmp, 100);
			dRadius = atof(szTmp);
			if (dRadius == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect radius!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			// check if already calculated
			if (dOldMass == dMass && dOldRadius == dRadius && dOldVelocity == dVelocity)
			{
				MessageBox(hDlg, TEXT("Already Computed!"), TEXT("Info"), MB_OK);
				return(TRUE);
			}

			// calculate result 
			dResult = CalculateCentripetalForce(dMass, dVelocity, dRadius);

			// create file to store result
			wsprintf(szTmp, TEXT("CentripetalForce%d.txt"), iNoOfFile++);
			hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			// display result 
			sprintf_s(szTmp, TEXT("Mass    : %g kg\nVelocity: %g m/s\nRadius  : %g m\n\nCentripetal Force: %g N"), dMass, dVelocity, dRadius, dResult);

			// display result in message box
			MessageBox(hDlg, TEXT(szTmp), TEXT("Message"), MB_OK | MB_ICONINFORMATION);

			// write result to file
			WriteFile(hFile, szTmp, strlen(szTmp), NULL, NULL);

			sprintf_s(szTmp, "%g", dResult);
			SetDlgItemText(hDlg, ID_CP_ETRESULT, szTmp);

			// set old state to these inputs
			dOldMass = dMass;
			dOldRadius = dRadius;
			dOldVelocity = dVelocity;

			return TRUE;

		case ID_CPA_COMPUTE:
			/*GetDlgItemText(hDlg, ID_CPA_ETMASS, szTmp, 100);
			dMass = atof(szTmp);
			if (dMass == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect mass!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}*/

			GetDlgItemText(hDlg, ID_CPA_ETVELOCITY, szTmp, 100);
			dVelocity = atof(szTmp);
			if (dVelocity == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect velocity!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			GetDlgItemText(hDlg, ID_CPA_ETRADIUS, szTmp, 100);
			dRadius = atof(szTmp);
			if (dRadius == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect radius!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			// check if already calculated
			if (dOldRadius == dRadius && dOldVelocity == dVelocity)
			{
				MessageBox(hDlg, TEXT("Already Computed!"), TEXT("Info"), MB_OK);
				return(TRUE);
			}

			// calculate result 
			dResult = CalculateCentripetalAcceleration(dVelocity, dRadius);

			// create file to store result
			wsprintf(szTmp, TEXT("CentripetalAcceleration%d.txt"), iNoOfFile++);
			hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			// display result 
			sprintf_s(szTmp, TEXT("Velocity: %g m/s\nRadius  : %g m\n\nCentripetal Acceleration: %g m/s²"), dVelocity, dRadius, dResult);

			// display result in message box
			MessageBox(hDlg, TEXT(szTmp), TEXT("Message"), MB_OK | MB_ICONINFORMATION);

			// write result to file
			WriteFile(hFile, szTmp, strlen(szTmp), NULL, NULL);

			sprintf_s(szTmp, "%g", dResult);
			SetDlgItemText(hDlg, ID_CPA_ETRESULT, szTmp);

			// set old state to these inputs
			dOldRadius = dRadius;
			dOldVelocity = dVelocity;
			break;

		case ID_CP_RESET:
		case ID_CPA_RESET:
			Reset(hDlg);
			break;

		case IDCANCEL:
			FreeLibrary(hDll);
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Reset(HWND hDlg)
{
	// Reset Centripetal
	SetDlgItemText(hDlg, ID_CP_ETMASS, TEXT(""));
	SetDlgItemText(hDlg, ID_CP_ETVELOCITY, TEXT(""));
	SetDlgItemText(hDlg, ID_CP_ETRADIUS, TEXT(""));
	SetDlgItemText(hDlg, ID_CP_ETRESULT, TEXT(""));

	// Reset Centrifugal
	SetDlgItemText(hDlg, ID_CPA_ETVELOCITY, TEXT(""));
	SetDlgItemText(hDlg, ID_CPA_ETRADIUS, TEXT(""));
	SetDlgItemText(hDlg, ID_CPA_ETRESULT, TEXT(""));
}

void Disable(HWND hDlg, int iChecked)
{
	HWND hControl = NULL;
	Reset(hDlg);

	if (iChecked == ID_RBCPFORCE)
	{
		// Enabe Centripetal controls 
		hControl = GetDlgItem(hDlg, ID_CP_ETMASS);
		EnableWindow(hControl, TRUE);

		

		hControl = GetDlgItem(hDlg, ID_CP_ETVELOCITY);
		EnableWindow(hControl, TRUE);

		hControl = GetDlgItem(hDlg, ID_CP_ETRADIUS);
		EnableWindow(hControl, TRUE);

		hControl = GetDlgItem(hDlg, ID_CP_COMPUTE);
		EnableWindow(hControl, TRUE);

		hControl = GetDlgItem(hDlg, ID_CP_RESET);
		EnableWindow(hControl, TRUE);

		// Disable Centrifugal controls
		/*hControl = GetDlgItem(hDlg, ID_CPA_ETMASS);
		EnableWindow(hControl, FALSE);*/

		hControl = GetDlgItem(hDlg, ID_CPA_ETVELOCITY);
		EnableWindow(hControl, FALSE);

		hControl = GetDlgItem(hDlg, ID_CPA_ETRADIUS);
		EnableWindow(hControl, FALSE);

		hControl = GetDlgItem(hDlg, ID_CPA_COMPUTE);
		EnableWindow(hControl, FALSE);

		hControl = GetDlgItem(hDlg, ID_CPA_RESET);
		EnableWindow(hControl, FALSE);

	}

	else if (iChecked == ID_RBCPACC)
	{
		// Enable Centrifugal controls
		/*hControl = GetDlgItem(hDlg, ID_CPA_ETMASS);
		EnableWindow(hControl, TRUE);*/

		hControl = GetDlgItem(hDlg, ID_CPA_ETVELOCITY);
		EnableWindow(hControl, TRUE);

		hControl = GetDlgItem(hDlg, ID_CPA_ETRADIUS);
		EnableWindow(hControl, TRUE);

		hControl = GetDlgItem(hDlg, ID_CPA_COMPUTE);
		EnableWindow(hControl, TRUE);

		hControl = GetDlgItem(hDlg, ID_CPA_RESET);
		EnableWindow(hControl, TRUE);

		// Disable Centripetal controls
		hControl = GetDlgItem(hDlg, ID_CP_ETMASS);
		EnableWindow(hControl, FALSE);	
									
		hControl = GetDlgItem(hDlg, ID_CP_ETVELOCITY);
		EnableWindow(hControl, FALSE);	
										
		hControl = GetDlgItem(hDlg, ID_CP_ETRADIUS);
		EnableWindow(hControl, FALSE);	
										
		hControl = GetDlgItem(hDlg, ID_CP_COMPUTE);
		EnableWindow(hControl, FALSE);	
										
		hControl = GetDlgItem(hDlg, ID_CP_RESET);
		EnableWindow(hControl, FALSE);
	}

	return;
}