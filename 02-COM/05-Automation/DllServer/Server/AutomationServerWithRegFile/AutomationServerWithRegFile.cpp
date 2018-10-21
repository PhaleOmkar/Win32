// header files
#include <Windows.h>
#include <stdio.h> // for swprintf_s() function 
#include "AutomationServerWithRegFile.h"

// CoClass declaration
class CMyMath : public IMyMath
{
private:
	long m_cRef;
	ITypeInfo *m_pITypeInfo = NULL;

public:
	CMyMath(void);
	~CMyMath(void);

	// IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IDispatch specific method declarations
	HRESULT __stdcall GetTypeInfoCount(UINT*);
	HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo**);
	HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*);
	HRESULT __stdcall Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

	// IMyMath specific method declarations
	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);

	// custom method
	HRESULT InitInstance(void);
};

// Class Factory Declaration
class CMyMathClassFactory : public IClassFactory
{
private:
	long m_cRef;

public: 
	CMyMathClassFactory(void);
	~CMyMathClassFactory(void);

	// IUnknow specific method declarations
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific method declarations
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// global Dll Handle 
HMODULE ghModule = NULL;

// global variable declaration
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

// GUID of LIBID {D772277D-DA91-4791-BEC4-56BD533C8446}
const GUID LIBID_AutomationServer = { 0xd772277d, 0xda91, 0x4791, 0xbe, 0xc4, 0x56, 0xbd, 0x53, 0x3c, 0x84, 0x46 };

// DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		ghModule = hDll;
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

// Implementation Of CMyMath methods
CMyMath::CMyMath(void)
{
	m_pITypeInfo = NULL;
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CMyMath::~CMyMath(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

// Implementation of CMyMath's IUnknown Interface methods
HRESULT CMyMath::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IMyMath *>(this);
	else if (riid == IID_IDispatch)
		*ppv = static_cast<IMyMath *>(this);
	else if (riid == IID_IMyMath)
		*ppv = static_cast<IMyMath *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CMyMath::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return(S_OK);
}

ULONG CMyMath::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		m_pITypeInfo->Release();
		m_pITypeInfo = NULL;
		delete(this);
		return(0);
	}

	return(m_cRef);
}

// Implementation of CMyMath's IMyMath interface methods
HRESULT CMyMath::SumOfTwoIntegers(int iNum1, int iNum2, int *pSum)
{
	*pSum = iNum1 + iNum2;
	return(S_OK);
}

HRESULT CMyMath::SubtractionOfTwoIntegers(int iNum1, int iNum2, int *pSub)
{
	*pSub = iNum1 - iNum2;
	return(S_OK);
}

HRESULT CMyMath::InitInstance(void)
{
	// function declaration
	void ComErrorDescriptionString(HWND, HRESULT);

	// variable declaration
	HRESULT hr;
	ITypeLib *pITypeLib = NULL;

	// code
	if (m_pITypeInfo == NULL)
	{
		hr = LoadRegTypeLib(LIBID_AutomationServer,
			1, 0, // major/minor version numbers 
			0x00,
			&pITypeLib);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(NULL, hr);
			return(hr);
		}

		hr = pITypeLib->GetTypeInfoOfGuid(IID_IMyMath, &m_pITypeInfo);
		if (FAILED(hr))
		{
			ComErrorDescriptionString(NULL, hr);
			pITypeLib->Release();
			return(hr);
		}

		pITypeLib->Release();
	}

	return(S_OK);
}

// Implementation of CMyMathClassFactory's methods
CMyMathClassFactory::CMyMathClassFactory(void)
{
	m_cRef = 1;
}

CMyMathClassFactory::~CMyMathClassFactory(void)
{
	// nothing here
}

// Implementation of CMyMathClassFactory's IUnknown interface methods
HRESULT CMyMathClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CMyMathClassFactory::AddRef(void)
{
	InterlockedIncrement(&glNumberOfActiveComponents);
	return(S_OK);
}

ULONG CMyMathClassFactory::Release(void)
{
	InterlockedDecrement(&glNumberOfServerLocks);

	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}

	return(m_cRef);
}

// Implementation of CMyMathClassFactory's IClassFactory interface methods
HRESULT CMyMathClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// Variable declaration
	CMyMath *pMyMath = NULL;
	HRESULT hr = S_OK;

	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	// Create instance of component
	pMyMath = new CMyMath;
	if (pMyMath == NULL)
		return(E_OUTOFMEMORY);

	// call automation related function
	pMyMath->InitInstance();

	// get the requested interface
	hr = pMyMath->QueryInterface(riid, ppv);
	
	pMyMath->Release();
	return(hr);
}

HRESULT CMyMathClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

// Implementation of CMyMathClassFactory's IDispatch interface methods
HRESULT CMyMath::GetTypeInfoCount(UINT *pCountTypeInfo)
{
	// as we have type library it is 1, else 0
	*pCountTypeInfo = 1;
	return(S_OK);
}

HRESULT CMyMath::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo **ppITypeInfo)
{
	*ppITypeInfo = NULL;

	if (iTypeInfo != 0)
		return(DISP_E_BADINDEX);

	m_pITypeInfo->AddRef();

	*ppITypeInfo = m_pITypeInfo;

	return(S_OK);
}

HRESULT CMyMath::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	return(DispGetIDsOfNames(m_pITypeInfo, rgszNames, cNames, rgDispId));
}

HRESULT CMyMath::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	// variable declaration
	HRESULT hr;

	hr = DispInvoke(this, m_pITypeInfo, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	return(hr);
}

// Implementation of exported functions from this Dll
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variable declaration
	CMyMathClassFactory *pMyMathsClassFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_MyMath)
		return(CLASS_E_CLASSNOTAVAILABLE);

	// create class factory
	pMyMathsClassFactory = new CMyMathClassFactory;

	if (pMyMathsClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pMyMathsClassFactory->QueryInterface(riid, ppv);

	pMyMathsClassFactory->Release();
	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}

void ComErrorDescriptionString(HWND hWnd, HRESULT hr)
{
	// variable declaration
	TCHAR *szErrorMessage = NULL;
	TCHAR str[255];

	if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
		hr = HRESULT_CODE(hr);
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&szErrorMessage, 0, NULL) != 0)
	{
		sprintf_s(str, "%#x : %s", hr, szErrorMessage);
		LocalFree(szErrorMessage);
	}
	else
		sprintf_s(str, "Could not found any description for issue");


	MessageBox(hWnd, str, TEXT("COM Error"), MB_OK);

}

