#define UNICODE

#include <Windows.h>
#include <TlHelp32.h>  // for process snapshot related APIs and Structures 
#include "ExeServer.h"

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD GetParentProcessID(void);

// class declaration
class CSumSubtract : public ISum, ISubtract
{
private:
	long m_cRef;

public:
	CSumSubtract(void);
	~CSumSubtract(void);

	// IUnknown Interface methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISum Interface methods
	HRESULT __stdcall SumOfTwoIntegers(int, int, int *);

	// ISubtract Interface methods
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *);
};

class CSumSubtractClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	CSumSubtractClassFactory(void);
	~CSumSubtractClassFactory(void);

	// IUknown Interface methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory Interface methods
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void **);
	HRESULT __stdcall LockServer(BOOL);

};

// global variable declaration
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
IClassFactory *gpIClassFactory = NULL;
HWND gHwnd = NULL;
DWORD dwRegister;

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	// function declarations
	HRESULT StartMyClassFactories(void);
	void StopMyClassFactories(void);
	DWORD GetParentProcessID(void);

	// variable declarations
	WNDCLASSEX wndclass;
	MSG msg;
	HWND hwnd;
	HRESULT hr;
	int iDontShowWindow = 0; // 0 means show the window
	TCHAR szClassName[] = TEXT("EXESERVER");
	TCHAR szTokens[] = TEXT("-/");
	TCHAR *pszTokens;
	TCHAR lpszCmdLine[255];
	wchar_t *next_token = NULL;

	// COM Library initialization
	GetParentProcessID();

	hr = CoInitialize(NULL);

	if (FAILED(hr))
		return(0);

	MultiByteToWideChar(CP_ACP, 0, lpCmdLine, 255, lpszCmdLine, 255);

	pszTokens = wcstok_s(lpszCmdLine, szTokens, &next_token);

	while (pszTokens != NULL)
	{
		if (_wcsicmp(pszTokens, TEXT("Embedding")) == 0)  // i.e. COM is calling me
		{
			iDontShowWindow = 1;  // dont show window, but message loop is must!
			break;
		}

		else 
		{
			exit(0);
		}

		pszTokens = wcstok_s(NULL, szTokens, &next_token);
	}

	// Window code
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// retgister class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szClassName, TEXT("Exe Server with Reg File"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	// initialize global window handle
	gHwnd = hwnd;

	if (iDontShowWindow != 1)   // TRUE if server is not called by COM Engine
	{
		ShowWindow(hwnd, iCmdShow);
		UpdateWindow(hwnd);

		// Increment server lock
		++glNumberOfServerLocks;
	}

	if (iDontShowWindow == 1) // TRUE if server is called by COM Engine
	{
		// start class factory
		hr = StartMyClassFactories();

		if (FAILED(hr))
		{
			DestroyWindow(hwnd);
			exit(0);
		}
	}

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (iDontShowWindow == 1)   // Only when COM calls the server
	{
		// stop class factories
		StopMyClassFactories();
	}

	// COM library uninitialization
	CoUninitialize();

	return((int)msg.wParam);
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declaration
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(0, 255, 0));
		DrawText(hdc, TEXT("This is a COM exe Server Program. Not For You!!"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
			PostQuitMessage(0);
		break;

	case WM_CLOSE:
		--glNumberOfServerLocks;
		ShowWindow(hwnd, SW_HIDE);
		// fall through, hence no break

	default:
		return(DefWindowProc(hwnd, iMsg, wParam, lParam));
	}

	return(0L);
}


// Implementation of CSumSubtrac's methods
CSumSubtract::CSumSubtract(void)
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

// Implementation Of CSumSubtract's IUnknown Interface
HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<ISum *>(this);
	else if (riid == IID_ISum)
		*ppv = static_cast<ISum *>(this);
	else if (riid == IID_ISubtract)
		*ppv = static_cast<ISubtract *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CSumSubtract::AddRef(void)
{
	InterlockedIncrement(&glNumberOfActiveComponents);
	return(m_cRef);
}

ULONG CSumSubtract::Release(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);

	if (m_cRef == 0)
	{
		delete(this);

		if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
			PostMessage(gHwnd, WM_QUIT, (WPARAM)0, (LPARAM)0L);

		return(0);
	}
	return(m_cRef);
}

// Implementation of CSumSubtract's ISum Interface
HRESULT CSumSubtract::SumOfTwoIntegers(int iNum1, int iNum2, int *pSum)
{
	*pSum = iNum1 + iNum2;
	return(S_OK);
}

// Implementation of CSumSubtract's ISubtract Interface
HRESULT CSumSubtract::SubtractionOfTwoIntegers(int iNum1, int iNum2, int *pSubtract)
{
	*pSubtract = iNum1 - iNum2;
	return(S_OK);
}

// Implementation Of CSumSubtract's methods
CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
	m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory(void)
{

}

// Implementation Of CSumSubtract's IUnknown Interface
HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory *>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CSumSubtractClassFactory::AddRef(void)
{
	InterlockedIncrement(&glNumberOfActiveComponents);
	return(m_cRef);
}

ULONG CSumSubtractClassFactory::Release()
{
	InterlockedDecrement(&glNumberOfActiveComponents);

	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}

	return(m_cRef);
}

// Implementation Of CSumSubtract's IClassFactory Intertface
HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// variable declarations
	CSumSubtract *pSumSubtract = NULL;
	HRESULT hr;

	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	// create the instance of component i.e. of CSumSubtract class
	pSumSubtract = new CSumSubtract;
	if (pSumSubtract == NULL)
		return(E_OUTOFMEMORY);

	// get the requested interface
	hr = pSumSubtract->QueryInterface(riid, ppv);
	pSumSubtract->Release();

	return(hr);
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

HRESULT StartMyClassFactories(void)
{
	// variable declarations 
	HRESULT hr;

	gpIClassFactory = new CSumSubtractClassFactory;
	if (gpIClassFactory == NULL)
		return(E_OUTOFMEMORY);

	gpIClassFactory->AddRef();

	// register the class factory in COM's private database
	hr = CoRegisterClassObject(CLSID_CSumSubtract, static_cast<IUnknown *>(gpIClassFactory), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &dwRegister);

	if (FAILED(hr))
	{
		gpIClassFactory->Release();
		return(E_FAIL);
	}

	return(S_OK);
}

void StopMyClassFactories(void)
{
	// un-register the class factory
	if (dwRegister != 0)
		CoRevokeClassObject(dwRegister);

	if (gpIClassFactory != NULL)
		gpIClassFactory->Release();
}

DWORD GetParentProcessID(void)
{
	// variable declaration
	HANDLE hProcessSnapshot = NULL;
	BOOL bRetCode = FALSE;
	PROCESSENTRY32 ProcessEntry = { 0 };
	DWORD dwPPID = 0;
	TCHAR szNameOfThisProcess[_MAX_PATH], szNameOfParentProcess[_MAX_PATH];
	TCHAR szTemp[_MAX_PATH], str[_MAX_PATH], *ptr = NULL;

	// first take current system snapshot
	hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnapshot == INVALID_HANDLE_VALUE)
		return(-1);

	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	// walk process hierarchy
	if (Process32First(hProcessSnapshot, &ProcessEntry))
	{
		GetModuleFileName(NULL, szTemp, _MAX_PATH);
		ptr = wcsrchr(szTemp, '\\');
		wcscpy_s(szNameOfThisProcess, ptr + 1);
		do
		{
			errno_t err;

			err = _wcslwr_s(szNameOfThisProcess, wcslen(szNameOfThisProcess) + 1);
			err = _wcsupr_s(ProcessEntry.szExeFile, wcslen(ProcessEntry.szExeFile) + 1);

			if (wcsstr(szNameOfThisProcess, ProcessEntry.szExeFile) != NULL)
			{
				wsprintf(str, TEXT("Current Process Name = %s\nCurrent Process ID = %ld\nParent Process ID = %ld\nParent Process Name = %s"),
					szNameOfThisProcess, ProcessEntry.th32ProcessID, ProcessEntry.th32ParentProcessID, ProcessEntry.szExeFile);
				MessageBox(NULL, TEXT("Parent Info"), str, MB_OK | MB_TOPMOST);
				dwPPID = ProcessEntry.th32ParentProcessID;
			}
		} while (Process32Next(hProcessSnapshot, &ProcessEntry));
	}

	CloseHandle(hProcessSnapshot);
	return(dwPPID);
}

