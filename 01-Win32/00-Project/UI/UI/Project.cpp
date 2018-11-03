#pragma region Headers

#include <Windows.h>
#include <stdio.h>
#include "Project.h"
#include "ChemistryCompounds.h"

#pragma endregion

#pragma region Global Function Declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK PhyDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK ChemDlgProc(HWND, UINT, WPARAM, LPARAM);

// For Physics
void PhyDisable(HWND, int);
void PhyReset(HWND);

// For Chemistry
void ChemReset(HWND);

#pragma endregion

#pragma region External Functions
// For Physics
typedef double(*lpfnCalculateCentripetalForce) (double, double, double);
typedef double(*lpfnCalculateCentripetalAcceleration) (double, double);

// For Chemistry
typedef double(*lpfnCalculateNumberOfMolecules) (double, double);
typedef double(*lpfnCalculateNumberOfAtoms) (double, double, int);
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

			DrawText(hdc, TEXT("Press       to Continue"), -1, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);

			SetTextColor(hdc, RGB(255, 0, 0));
			//DrawText(hdc, TEXT("\n\n\n\n\n              P                   "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n                  C               "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n              M                   "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n                  B               "), -1, &rc, DT_VCENTER | DT_LEFT);
			//DrawText(hdc, TEXT("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n              Q               "), -1, &rc, DT_VCENTER | DT_LEFT);

			DrawText(hdc, TEXT("      Space            "), -1, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
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
				DialogBox(hInstance, TEXT("DLGMATHS"), hDlg, PhyDlgProc);
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

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case ID_RBCPFORCE:
		case ID_RBCPACC:
			iForce = LOWORD(wParam);
			PhyDisable(hDlg, iForce);
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
			PhyReset(hDlg);
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
	double dResult = 0.0;
	double dResultAtoms = 0.0;

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
		for (int i = 0; i < iNoOfCompounds; i++) 
		{
			int pos = SendMessage(hwndList, CB_ADDSTRING, 0, (LPARAM) arrCompounds[i].szName);
			SendMessage(hwndList, CB_SETITEMDATA, pos, (LPARAM)arrCompounds[i].iIndex);
		}

		return TRUE;
	#pragma endregion
	
	#pragma region WM_COMMAND
	case WM_COMMAND:
		
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
			wsprintf(szTmp, TEXT("NumberOfAtomsAndMolecules%d.txt"), iNoOfFile++);
			hFile = CreateFile(szTmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			// display result 
			sprintf_s(szTmp, TEXT("Molecular Mass      : %g g/mol\nAmount Of Substance : %g g\n\nNumber Of Atoms     : %g\nNumber of Molecules : %g "), dMolecularMass, dAmountOfSubstance, dResultAtoms, dResult);

			// display result in message box
			MessageBox(hDlg, TEXT(szTmp), TEXT("Message"), MB_OK | MB_ICONINFORMATION);

			// write result to file
			WriteFile(hFile, szTmp, strlen(szTmp), NULL, NULL);

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

		break;
	#pragma endregion
	}

	return FALSE;
	
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
#pragma endregion