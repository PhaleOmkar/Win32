#pragma region Headers

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "Project.h"
#include "ChemistryCompounds.h"

#pragma endregion

#pragma region Global Function Declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK PhyDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK ChemDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK MathsDlgProc(HWND, UINT, WPARAM, LPARAM);

// For Physics
void PhyDisable(HWND, int);
void PhyReset(HWND);

// For Chemistry
void ChemReset(HWND);
void ChemDisable(HWND, BOOL);

// For Mathematics
void MathsReset(HWND);

// Helper Functions
void MakeFullScreen(HWND);

#pragma endregion

#pragma region External Functions
// For Physics
typedef double(*lpfnCalculateCentripetalForce) (double, double, double);
typedef double(*lpfnCalculateCentripetalAcceleration) (double, double);

// For Chemistry
typedef double(*lpfnCalculateNumberOfMolecules) (double, double);
typedef double(*lpfnCalculateNumberOfAtoms) (double, double, int);

// For Maths
typedef long(*lpfnCalculateRootsOfEq) (double, double, double, double *, double *);
#pragma endregion

#pragma region Main Window
// WinMain
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

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL fIsSplashScreen = TRUE;
	static int cxClient, cyClient;
	static HBITMAP hbmpImage = NULL;
	int iX = 0, iY = 0;

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
		MakeFullScreen(hwnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			fIsSplashScreen = FALSE;
			
			GetClientRect(hwnd, &rc);
			InvalidateRect(hwnd, &rc, TRUE);

			DialogBox(hInstance, TEXT("DLGMAIN"), hwnd, MainDlgProc);
			
			fIsSplashScreen = TRUE;
			InvalidateRect(hwnd, &rc, TRUE);
			
			break;

			/*case 'P':
				DialogBox(hInstance, TEXT("DLGPHY"), hwnd, PhyDlgProc);
				break;

			case 'C':
				DialogBox(hInstance, TEXT("DLGCHEM"), hwnd, PhyDlgProc);
				break;

			case 'M':
				DialogBox(hInstance, TEXT("DLGMATHS"), hwnd, PhyDlgProc);
				break;

			case 'B':
				DialogBox(hInstance, TEXT("DLGBIO"), hwnd, PhyDlgProc);
				break;*/

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
		
		// calculate points so that image will be in the center of the display
		iX = (cxClient / 2) - (bmpImage.bmWidth / 2);
		iY = (cyClient / 2) - (bmpImage.bmHeight / 2);
		
		BitBlt(hdc, iX, iY, bmpImage.bmWidth, bmpImage.bmHeight, hdcCompatible, 0, 0, SRCCOPY);
		DeleteDC(hdcCompatible);

		GetClientRect(hwnd, &rc);
		hFont = CreateFont(30, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Consolas"));
		SelectObject(hdc, hFont);
		SetBkMode(hdc, TRANSPARENT);

		if (fIsSplashScreen)
		{
			SetTextColor(hdc, RGB(255, 255, 255));
			//DrawText(hdc, TEXT("\n\n\n\n\n        Press    for Physics      "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n            Press    for Chemistry"), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n        Press    for Mathematics  "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n            Press    for Biology  "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n        Press    to Quit      "), -1, &rc, DT_VCENTER | DT_LEFT);

			DrawText(hdc, TEXT("Press       to Continue\n"), -1, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
			DrawText(hdc, TEXT("\n\n    ASTROMEDICOMP"), -1, &rc, DT_LEFT);
			DrawText(hdc, TEXT("\n\n    Rutwik Choughule    "), -1, &rc, DT_RIGHT);

			SetTextColor(hdc, RGB(255, 0, 0));
			//DrawText(hdc, TEXT("\n\n\n\n\n              P                   "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n                  C               "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n              M                   "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n                  B               "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n              Q               "), -1, &rc, DT_VCENTER | DT_LEFT);

			DrawText(hdc, TEXT("      Space            \n"), -1, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
			DrawText(hdc, TEXT("\n    GROUP: WM_APP"), -1, &rc, DT_LEFT);
			DrawText(hdc, TEXT("\n       WinRT 2018       "), -1, &rc, DT_RIGHT);

		}
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

// Main Dialog Procedure
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	static int iSubject;

	switch (iMsg)
	{
	case WM_INITDIALOG:

		// get hInstance
		hInstance = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);

		// set default radio button to Physics
		iSubject = ID_MAIN_RB_PHY;
		CheckRadioButton(hDlg, ID_MAIN_RB_PHY, ID_MAIN_RB_BIO, iSubject);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MAIN_RB_PHY:
		case ID_MAIN_RB_CHEM:
		case ID_MAIN_RB_MATHS:
		case ID_MAIN_RB_BIO:
			iSubject = LOWORD(wParam);
			return TRUE;

		case ID_MAIN_OK:

			switch (iSubject)
			{
			case ID_MAIN_RB_PHY:
				DialogBox(hInstance, TEXT("DLGPHY"), hDlg, PhyDlgProc);
				break;

			case ID_MAIN_RB_CHEM:
				DialogBox(hInstance, TEXT("DLGCHEM"), hDlg, ChemDlgProc);
				break;

			case ID_MAIN_RB_MATHS:
				DialogBox(hInstance, TEXT("DLGMATHS"), hDlg, MathsDlgProc);
				break;

			case ID_MAIN_RB_BIO:
				DialogBox(hInstance, TEXT("DLGBIO"), hDlg, PhyDlgProc);
				break;
			}

			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		return TRUE;

	}

	return FALSE;
}
#pragma endregion

#pragma region Physics
// Physics Dialog Procedure
BOOL CALLBACK PhyDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
	char szHTML[1024] = "";
	double dResult = 0.0;
	HDC hDC = NULL;
	HWND hControl = NULL;

	switch (iMsg)
	{
	#pragma region WM_INITDIALOG
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
		PhyDisable(hDlg, iForce);

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
			MessageBox(hDlg, TEXT("Unable to load Physics DLL"), TEXT("Error"), MB_OK);
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
	#pragma endregion

	#pragma region WM_COMMAND
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case ID_RBCPFORCE:
		case ID_RBCPACC:
			iForce = LOWORD(wParam);
			PhyDisable(hDlg, iForce);
			dOldMass = dOldRadius = dOldVelocity = 0.0;
			return TRUE;

		#pragma region ID_CP_COMPUTE
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
			wsprintf(szTmp, TEXT("CentripetalForce%d.html"), iNoOfFile++);
			hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						
			// display result 
			sprintf_s(szTmp, TEXT("Mass    : %g kg\nVelocity: %g m/s\nRadius  : %g m\n\nCentripetal Force: %g N"), dMass, dVelocity, dRadius, dResult);

			// display result in message box
			MessageBox(hDlg, TEXT(szTmp), TEXT("Message"), MB_OK | MB_ICONINFORMATION);
					
			sprintf_s(szHTML, "<!DOCTYPE html><html><head><title>%s</title></head><body style=\"background: black; color: white; font - family: monospace; \"> <Center><div><h1 style=\"color: red\">%s</h1><table cellspacing=\"10\" style=\"border - spacing : 20px\"><tbody ><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr></tbody></table></div></Center></body></html>", "Physics", "Centripetal Force", "Mass", dMass, "Velocity", dVelocity, "Radius", dRadius, "Centripetal Force", dResult);

			// write result to file
			WriteFile(hFile, szHTML, strlen(szHTML), NULL, NULL);
			CloseHandle(hFile);

			sprintf_s(szTmp, "%g", dResult);
			SetDlgItemText(hDlg, ID_CP_ETRESULT, szTmp);

			// set old state to these inputs
			dOldMass = dMass;
			dOldRadius = dRadius;
			dOldVelocity = dVelocity;

			return TRUE;

		#pragma endregion

		#pragma region ID_CPA_COMPUTE
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
			wsprintf(szTmp, TEXT("CentripetalAcceleration%d.html"), iNoOfFile++);
			hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			// display result 
			sprintf_s(szTmp, TEXT("Velocity: %g m/s\nRadius  : %g m\n\nCentripetal Acceleration: %g m/s²"), dVelocity, dRadius, dResult);

			// display result in message box
			MessageBox(hDlg, TEXT(szTmp), TEXT("Message"), MB_OK | MB_ICONINFORMATION);

			sprintf_s(szHTML, "<!DOCTYPE html><html><head><title>%s</title></head><body style=\"background: black; color: white; font - family: monospace; \"> <Center><div><h1 style=\"color: red\">%s</h1><table cellspacing=\"10\" style=\"border - spacing : 20px\"><tbody ><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr></tbody></table></div></Center></body></html>", "Physics", "Centripetal Acceleration", "Velocity", dVelocity, "Radius", dRadius, "Centripetal Acceleration", dResult);

			// write result to file
			WriteFile(hFile, szHTML, strlen(szHTML), NULL, NULL);
			CloseHandle(hFile);

			sprintf_s(szTmp, "%g", dResult);
			SetDlgItemText(hDlg, ID_CPA_ETRESULT, szTmp);

			// set old state to these inputs
			dOldRadius = dRadius;
			dOldVelocity = dVelocity;
			break;
		#pragma endregion

		case ID_CP_RESET:
		case ID_CPA_RESET:
			PhyReset(hDlg);
			break;

		case IDCANCEL:
			FreeLibrary(hDll);
			EndDialog(hDlg, 0);
			return TRUE;
		}
		return TRUE;
	#pragma endregion
	}

	return FALSE;

}

// Physics Related functions
void PhyReset(HWND hDlg)
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

void PhyDisable(HWND hDlg, int iChecked)
{
	HWND hControl = NULL;
	PhyReset(hDlg);

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
#pragma endregion

#pragma region Chemistry
// Chemistry Dialog Procedure
BOOL CALLBACK ChemDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HMODULE hDll = NULL;
	static HANDLE hFile = NULL;
	HWND hwndList = NULL;
	static int iNoOfFile = 0;
	static lpfnCalculateNumberOfMolecules CalculateNumberOfMolecules = NULL;
	static lpfnCalculateNumberOfAtoms CalculateNumberOfAtoms = NULL;
	static int iOldNoOfAtomsInMolecule;
	static double dOldMolecularMass;
	static double dOldAmountOfSubstance;
	int iNoOfAtomsInMolecule;
	double dMolecularMass;
	double dAmountOfSubstance;
	char szTmp[255] = "";
	char szHTML[1024] = "";
	double dResult = 0.0;
	double dResultAtoms = 0.0;
	int pos = 0;

	switch (iMsg)
	{
	#pragma region WM_INITDIALOG
	case WM_INITDIALOG:
		// initialize variable
		iNoOfAtomsInMolecule = 0;
		dOldMolecularMass = 0.0;
		dOldAmountOfSubstance = 0.0;
		iOldNoOfAtomsInMolecule = 0;
		dMolecularMass = 0.0;
		dAmountOfSubstance = 0.0;

		// Load Physics DLL
		hDll = LoadLibrary(TEXT("ChemistryHelper.dll"));
		if (hDll == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to load Chemistry DLL"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		CalculateNumberOfMolecules = (lpfnCalculateNumberOfMolecules)GetProcAddress(hDll, "CalculateNumberOfMolecules");
		if (CalculateNumberOfMolecules == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to locate CalculateNumberOfMolecules function"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		CalculateNumberOfAtoms = (lpfnCalculateNumberOfAtoms)GetProcAddress(hDll, "CalculateNumberOfAtoms");
		if (CalculateNumberOfAtoms == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to locate CalculateNumberOfAtoms function"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		// fill the combo box for predefined compounds
		hwndList = GetDlgItem(hDlg, ID_CHEM_CMBCMPDS);

		pos = SendMessage(hwndList, CB_ADDSTRING, 0, (LPARAM)"--- Custom ---");
		SendMessage(hwndList, CB_SETITEMDATA, pos, (LPARAM)0);

		for (int i = 0; i < iNoOfCompounds; i++)
		{
			pos = SendMessage(hwndList, CB_ADDSTRING, 0, (LPARAM)arrCompounds[i].szName);
			SendMessage(hwndList, CB_SETITEMDATA, pos, (LPARAM)arrCompounds[i].iIndex);
		}

		SendMessage(hwndList, CB_SETCURSEL, (WPARAM)0, 0);

		return TRUE;
#pragma endregion

	#pragma region WM_COMMAND
	case WM_COMMAND:

		switch (HIWORD(wParam))
		{
		case  CBN_SELCHANGE:
			// get the selected index from the combo box list
			pos = (int)SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (pos == 0)
				ChemDisable(hDlg, FALSE);
			else
			{
				ChemDisable(hDlg, TRUE);

				sprintf_s(szTmp, "%g", arrCompounds[pos - 1].dMolecularMass);
				SetDlgItemText(hDlg, ID_CHEM_ETMMASS, szTmp);

				sprintf_s(szTmp, "%d", arrCompounds[pos - 1].iNoOfAtoms);
				SetDlgItemText(hDlg, ID_CHEM_ETNATOMS, szTmp);

			}

			return TRUE;
		}

		switch (LOWORD(wParam))
		{

		case ID_CHEM_CALCULATE:

			// Get Molecular Mass
			GetDlgItemText(hDlg, ID_CHEM_ETMMASS, szTmp, 100);
			dMolecularMass = atof(szTmp);
			if (dMolecularMass == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect Molecular Mass!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			// Get Amount Of Substance
			GetDlgItemText(hDlg, ID_CHEM_ETAMOUNT, szTmp, 100);
			dAmountOfSubstance = atof(szTmp);
			if (dAmountOfSubstance == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect Amount Of Substance!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			// Get Number of Atoms in one Molecule
			GetDlgItemText(hDlg, ID_CHEM_ETNATOMS, szTmp, 100);
			iNoOfAtomsInMolecule = atoi(szTmp);
			if (iNoOfAtomsInMolecule == 0)
			{
				MessageBox(hDlg, TEXT("Incorrect Amount Of Substance!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			// Check if already computed
			if ((dOldAmountOfSubstance == dAmountOfSubstance) && (dOldMolecularMass == dMolecularMass) && (iOldNoOfAtomsInMolecule == iNoOfAtomsInMolecule))
			{
				MessageBox(hDlg, TEXT("Already Computed!"), TEXT("Info"), MB_OK);
				return(TRUE);
			}

			// Compute the value
			dResult = CalculateNumberOfMolecules(dMolecularMass, dAmountOfSubstance);
			dResultAtoms = CalculateNumberOfAtoms(dMolecularMass, dAmountOfSubstance, iNoOfAtomsInMolecule);

			// create file to store result
			wsprintf(szTmp, TEXT("NumberOfAtomsAndMolecules%d.html"), iNoOfFile++);
			hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			// display result 
			sprintf_s(szTmp, TEXT("Molecular Mass      : %g g/mol\nAmount Of Substance : %g g\n\nNumber Of Atoms     : %g\nNumber of Molecules : %g "), dMolecularMass, dAmountOfSubstance, dResultAtoms, dResult);

			// display result in message box
			MessageBox(hDlg, TEXT(szTmp), TEXT("Message"), MB_OK | MB_ICONINFORMATION);
			
			sprintf_s(szHTML, 
				"<!DOCTYPE html><html><head><title>%s</title></head><body style=\"background: black; color: white; font - family: monospace; \"> <Center><div><h1 style=\"color: red\">%s</h1><table cellspacing=\"10\" style=\"border - spacing : 20px\"><tbody ><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%g</h3></td> </tr></tbody></table></div></Center></body></html>", "Chemistry", "Number Of Atoms And Molecules",
				"Molecular Mass", dMolecularMass, "Amount Of Substance", dAmountOfSubstance, "Number Of Atoms", dResultAtoms, "Number Of Molecules", dResult);

			// write result to file
			WriteFile(hFile, szHTML, strlen(szHTML), NULL, NULL);
			CloseHandle(hFile);

			sprintf_s(szTmp, "%g", dResult);
			SetDlgItemText(hDlg, ID_CHEM_LRESULT, szTmp);

			sprintf_s(szTmp, "%g", dResultAtoms);
			SetDlgItemText(hDlg, ID_CHEM_LRESULTATOM, szTmp);

			// Set Old state
			dOldAmountOfSubstance = dAmountOfSubstance;
			dOldMolecularMass = dMolecularMass;
			iNoOfAtomsInMolecule = iOldNoOfAtomsInMolecule;

			return TRUE;

		case ID_CHEM_RESET:
			ChemReset(hDlg);
			return TRUE;

		case IDCANCEL:
			FreeLibrary(hDll);
			EndDialog(hDlg, 0);
			return TRUE;
		}

		return TRUE;
	#pragma endregion
	}
	
	return(FALSE);
}

// Chemistry Related functions
void ChemReset(HWND hDlg)
{
	// Reset All Fields
	SetDlgItemText(hDlg, ID_CHEM_ETMMASS,		TEXT(""));
	SetDlgItemText(hDlg, ID_CHEM_ETAMOUNT,		TEXT(""));
	SetDlgItemText(hDlg, ID_CHEM_ETNATOMS,		TEXT(""));
	SetDlgItemText(hDlg, ID_CHEM_LRESULTATOM,	TEXT(""));
	SetDlgItemText(hDlg, ID_CHEM_LRESULT,		TEXT(""));
}

void ChemDisable(HWND hDlg, BOOL fDisable)
{
	HWND hControl = NULL;
	ChemReset(hDlg);
	
	hControl = GetDlgItem(hDlg, ID_CHEM_ETMMASS);
	EnableWindow(hControl, !fDisable);
		
	hControl = GetDlgItem(hDlg, ID_CHEM_ETNATOMS);
	EnableWindow(hControl, !fDisable);
}

#pragma endregion

#pragma region Maths
// Maths Dialog Procedure
BOOL CALLBACK MathsDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HMODULE hDll = NULL;
	static lpfnCalculateRootsOfEq CalculateRootsOfEq = NULL;
	static HANDLE hFile = NULL;
	static int iNoOfFile = 0;
	double dA, dB, dC;
	double dRoot1, dRoot2;
	long lNoOfRoots;
	char szTmp[255] = "";
	char szTmp1[255] = "";
	char szHTML[1024] = "";
	char chSign1 = '+', chSign2 = '+';

	switch (iMsg)
	{
	#pragma region WM_INITDIALOG
	case WM_INITDIALOG:

		// Load Maths DLL
		hDll = LoadLibrary(TEXT("MathsHelper.dll"));
		if (hDll == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to load Maths DLL"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}

		// Get function pointer
		CalculateRootsOfEq = (lpfnCalculateRootsOfEq)GetProcAddress(hDll, "CalculateRootsOfEq");
		if (CalculateRootsOfEq == NULL)
		{
			MessageBox(hDlg, TEXT("Unable to locate CalculateRootsOfEq function"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, -1);
		}
		
		return TRUE;
	#pragma endregion

	#pragma region WM_COMMAND
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MATHS_SOLVE:

			// Get a
			GetDlgItemText(hDlg, ID_MATHS_A, szTmp, 100);
			dA = atof(szTmp);
			if (dA == 0.0)
			{
				MessageBox(hDlg, TEXT("Coefficient of x² (a) cannot be zero!"), TEXT("Error"), MB_OK);
				return(TRUE);
			}

			// Get b
			GetDlgItemText(hDlg, ID_MATHS_B, szTmp, 100);
			dB = atof(szTmp);
			if (dB == 0.0)
			{
				SetDlgItemText(hDlg, ID_MATHS_B, TEXT("0.0"));
			}

			// Get c
			GetDlgItemText(hDlg, ID_MATHS_C, szTmp, 100);
			dC = atof(szTmp);
			if (dC == 0.0)
			{
				SetDlgItemText(hDlg, ID_MATHS_C, TEXT("0.0"));
			}

			// Solve the equation! 
			lNoOfRoots = CalculateRootsOfEq(dA, dB, dC, &dRoot1, &dRoot2);

			
			// create file to store result
			wsprintf(szTmp, TEXT("RootsOfEqn%d.html"), iNoOfFile++);
			hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			
			switch (lNoOfRoots)
			{

			case 0:
				sprintf_s(szTmp, "Imaginary roots");
				break;

			case 1:
				sprintf_s(szTmp, "%g", dRoot1);
				break;

			case 2:
				sprintf_s(szTmp, "%g and %g", dRoot1, dRoot2);
				break;
			}

			// Write to HTML string
			if (dB < 0) chSign1 = '-';
			if (dC < 0) chSign2 = '-';
			sprintf_s(szTmp1, "%gx² %c %gx %c %g = 0", dA, chSign1, fabs(dB), chSign2, fabs(dC));

			sprintf_s(szHTML,
				"<!DOCTYPE html><html><head><title>%s</title></head><body style=\"background: black; color: white; font - family: monospace; \"> <Center><div><h1 style=\"color: red\">%s</h1><table cellspacing=\"10\" style=\"border - spacing : 20px\"><tbody ><tr> <td><h3>%s</h3></td> <td><h3>%s</h3></td> </tr><tr> <td><h3>%s</h3></td> <td><h3>%s</h3></td> </tr></tbody></table></div></Center></body></html>",
				"Mathematics", "Roots Of Quadratic Equation",
				"Equation", szTmp1, "Roots", szTmp);


			// write result to file
			WriteFile(hFile, szHTML, strlen(szHTML), NULL, NULL);
			CloseHandle(hFile);

			SetDlgItemText(hDlg, ID_MATHS_RESULT, szTmp);

			return TRUE;

		case ID_MATHS_RESET:
			MathsReset(hDlg);
			break;

		case IDCANCEL:
			FreeLibrary(hDll);
			EndDialog(hDlg, 0);
			return TRUE;
		}

		return TRUE;
	#pragma endregion


	}

	return FALSE;
}

// Maths Related functions
void MathsReset(HWND hDlg)
{
	// Reset All Fields
	SetDlgItemText(hDlg, ID_MATHS_A,		TEXT(""));
	SetDlgItemText(hDlg, ID_MATHS_B,		TEXT(""));

	SetDlgItemText(hDlg, ID_MATHS_C,		TEXT(""));
	SetDlgItemText(hDlg, ID_MATHS_RESULT,	TEXT(""));
}
#pragma endregion

#pragma region Helper Functions

void MakeFullScreen(HWND hwnd)
{
	DWORD dwStyle;
	MONITORINFO MI;

	dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	MI = { sizeof(MONITORINFO) };
	if (GetMonitorInfo(MonitorFromWindow(hwnd, MONITORINFOF_PRIMARY), &MI))
	{
		SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd,
			HWND_TOP,
			MI.rcMonitor.left,
			MI.rcMonitor.top,
			MI.rcMonitor.right - MI.rcMonitor.left,
			MI.rcMonitor.bottom - MI.rcMonitor.top,
			SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	
}

#pragma endregion
