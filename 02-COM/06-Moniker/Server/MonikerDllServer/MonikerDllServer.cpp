#include <Windows.h>
#include "MonikerDllServer.h"

// class declaration
class COddNumber : public IOddNumber
{
private:
	long m_cRef;
	int m_iFirstOddNumber;

public:
	COddNumber(int);
	~COddNumber(void);
	
	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// class's method
	BOOL __stdcall IsOdd(int);

	// IOddNumber specific method
	HRESULT __stdcall GetNextOddNumber(int *);

};

class COddNumberFactory : public IOddNumberFactory
{
private:
	long m_cRef;

public:
	COddNumberFactory(void);
	~COddNumberFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);
	
	// IOddNumberFactory specific method
	HRESULT __stdcall SetFirstOddNumber(int, IOddNumber **);
};

// global variable declaration
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
long ghModule = NULL;  // DLL instance handle

// DllMain
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID Reserved)
{
	// code
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return(TRUE);
}

// COddNumber methods
COddNumber::COddNumber(int iFirstOddNumber)
{
	m_cRef = 1;
	m_iFirstOddNumber = iFirstOddNumber;

	InterlockedIncrement(&glNumberOfActiveComponents);
}

COddNumber::~COddNumber(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

// Implementation of COddNumber's IUnknown's methods
HRESULT COddNumber::QueryInterface(REFIID riid, void **ppv)
{
	// code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IOddNumber *>(this);
	else if (riid == IID_IOddNumber)
		*ppv = static_cast<IOddNumber *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG COddNumber::AddRef(void)
{
	// code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG COddNumber::Release(void)
{
	// code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}
	return(m_cRef);
}

// implementation of custom method of this class
BOOL COddNumber::IsOdd(int iNumber)
{
	if (iNumber != 0 && iNumber % 2 != 0)
		return(TRUE);
	else
		return(FALSE);
}

// Implementation of COddNumber's IOddNumber's methods
HRESULT COddNumber::GetNextOddNumber(int *pNextOddNumber)
{
	// variables
	BOOL bResult;
	bResult = IsOdd(m_iFirstOddNumber);

	if (bResult == TRUE)
		*pNextOddNumber = m_iFirstOddNumber + 2;
	else
		return(S_FALSE);

	return(S_OK);
}

// COddNumberFactory methods
COddNumberFactory::COddNumberFactory(void)
{
	m_cRef = 1;
}

COddNumberFactory::~COddNumberFactory(void)
{

}

// Implementation of COddNumber's IUnknown's methods
HRESULT COddNumberFactory::QueryInterface(REFIID riid, void **ppv)
{
	// code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IOddNumberFactory *>(this);
	else if (riid == IID_IOddNumberFactory)
		*ppv = static_cast<IOddNumberFactory *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG COddNumberFactory::AddRef(void)
{
	// code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG COddNumberFactory::Release(void)
{
	// code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}
	return(m_cRef);
}

// Implementation of COddNumberFactory's IOddNumberFactory's methods
HRESULT COddNumberFactory::SetFirstOddNumber(int iFirstOddNumber, IOddNumber **ppIOddNumber)
{
	// variable declaration
	HRESULT hr;
	COddNumber *pCOddNumber = new COddNumber(iFirstOddNumber);

	if (pCOddNumber == NULL)
		return(E_OUTOFMEMORY);

	hr = pCOddNumber->QueryInterface(IID_IOddNumber, (void **) ppIOddNumber);
	pCOddNumber->Release();

	return(hr);
}

// Exported functions from dll
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variable declaration
	COddNumberFactory *pCOddNumberFactory = NULL;
	HRESULT hr;

	// code
	if (rclsid != CLSID_OddNumber)
		return(CLASS_E_CLASSNOTAVAILABLE);

	// create class factory
	pCOddNumberFactory = new COddNumberFactory;
	if (pCOddNumberFactory == NULL)
		return(E_OUTOFMEMORY);
	hr = pCOddNumberFactory->QueryInterface(riid, ppv);
	pCOddNumberFactory->Release(); 
	return(hr);
}

HRESULT __stdcall DllCanUnloadNow(void)
{
	// code
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}




