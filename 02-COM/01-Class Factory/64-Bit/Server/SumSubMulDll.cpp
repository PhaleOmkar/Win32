#define UNICODE
#include <Windows.h>
#include "SumSubMulDll.h"

// Class Declaration
class CSumSubMul : public ISum, ISubtract, IMultiply
{
private:
	long m_cRef;

public:
	CSumSubMul(void);
	~CSumSubMul(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISum specific methods
	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);

	// ISubtract specific methods
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);

	// IMultiply specific methods
	HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*);
};

class CSumSubMulClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	CSumSubMulClassFactory(void);
	~CSumSubMulClassFactory(void);

	// IUnknown specific method declaration (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific method declaration (inherited)
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// global variable declaration
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

// DLL main
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

// Implementation of CSumSubMul's constructor method
CSumSubMul::CSumSubMul(void)
{
	// code
	m_cRef = 1; // hardcoded initialization to anticipate possible failure of QueryInterface
	InterlockedIncrement(&glNumberOfActiveComponents); // Increment global counter
}

// Implementation of CSumSubMul's destructor method
CSumSubMul::~CSumSubMul(void)
{
	// code
	InterlockedDecrement(&glNumberOfActiveComponents); // decrement global counter
}

// Implementation of CSumSubMul's IUnknown's methods
HRESULT CSumSubMul::QueryInterface(REFIID riid, void **ppv)
{
	// code
	if (riid == IID_IUnknown)
		*ppv = static_cast<ISum *>(this);
	else if (riid == IID_ISum)
		*ppv = static_cast<ISum *>(this);
	else if (riid == IID_ISubtract)
		*ppv = static_cast<ISubtract *>(this);
	else if (riid == IID_IMultiply)
		*ppv = static_cast<IMultiply *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CSumSubMul::AddRef(void)
{
	// code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CSumSubMul::Release(void)
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

// Implementation of ISum's methods
HRESULT CSumSubMul::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
	*pSum = num1 + num2;
	return(S_OK);
}

// Implementation of ISubtract's methods
HRESULT CSumSubMul::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
	*pSubtract = num1 - num2;
	return(S_OK);
}

// Implementation of IMultiply's methods
HRESULT CSumSubMul::MultiplicationOfTwoIntegers(int num1, int num2, int *pMultiplication)
{
	*pMultiplication = num1 * num2;
	return(S_OK);
}

// Implementation of CSumSubMulClassFactory's Constructor Methods
CSumSubMulClassFactory::CSumSubMulClassFactory(void)
{
	// code
	m_cRef = 1; // hardcoded initialization to anticipate failure of QueryInterface()
}

// Implementation of CSumSubMulClassFactory's Destructor Methods
CSumSubMulClassFactory::~CSumSubMulClassFactory(void)
{
	// code
}

// Implementation of CSumSubMulClassFactory's IClassFactory's IUnknown's Methods
HRESULT CSumSubMulClassFactory::QueryInterface(REFIID riid, void ** ppv)
{
	// code
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

ULONG CSumSubMulClassFactory::AddRef(void)
{
	// code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CSumSubMulClassFactory::Release(void)
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

// Implementation of IClassFactory's methods
HRESULT CSumSubMulClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// variables
	CSumSubMul *pCSumSubMul = NULL;
	HRESULT hr = S_OK;

	// code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	// Create instance of CSumSubMul class
	pCSumSubMul = new CSumSubMul;
	if (pCSumSubMul == NULL)
		return(E_OUTOFMEMORY);

	// get the requested interface
	hr = pCSumSubMul->QueryInterface(riid, ppv);
	pCSumSubMul->Release();
	return(hr);	
}

HRESULT CSumSubMulClassFactory::LockServer(BOOL fLock)
{
	// code
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

// Exported Functions from this DLL
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variable declaration
	CSumSubMulClassFactory *pCSumSubMulClassFactory = NULL;
	HRESULT hr = S_OK;

	// code
	if (rclsid != CLSID_SumSubMul)
		return(CLASS_E_CLASSNOTAVAILABLE);

	// create class factory
	pCSumSubMulClassFactory = new CSumSubMulClassFactory;
	if (pCSumSubMulClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pCSumSubMulClassFactory->QueryInterface(riid, ppv);
	pCSumSubMulClassFactory->Release();
	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}
