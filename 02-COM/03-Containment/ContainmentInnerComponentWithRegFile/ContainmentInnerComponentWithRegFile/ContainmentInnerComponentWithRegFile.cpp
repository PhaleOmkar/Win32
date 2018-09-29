#define UNICODE
#include <Windows.h>
#include "ContainmentInnerComponentWithRegFile.h"

class CMultiplicationDivision : public IMultiplication, IDivision
{
private:
	long m_cRef;

public: 
	CMultiplicationDivision(void); 
	~CMultiplicationDivision(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG   __stdcall AddRef(void);
	ULONG   __stdcall Release(void);

	// IMultiplication specific methods
	HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int *);

	// IDivision specific methods
	HRESULT __stdcall DivisionOfTwoIntegers(int, int, int *);
};

class CMultiplicationDivisionClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	CMultiplicationDivisionClassFactory(void);
	~CMultiplicationDivisionClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG   __stdcall AddRef(void);
	ULONG   __stdcall Release(void);

	// IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void **);
	HRESULT __stdcall LockServer(BOOL);
};

// global variables
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

// DllMain
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID Reserved)
{
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

// CMultiplicationDivision's method implementation
CMultiplicationDivision::CMultiplicationDivision(void)
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CMultiplicationDivision::~CMultiplicationDivision(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

// CMultiplicationDivision's IUnknown specific methods
HRESULT CMultiplicationDivision::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IMultiplication *>(this);
	else if (riid == IID_IMultiplication)
		*ppv = static_cast<IMultiplication *>(this);
	else if (riid == IID_IDivision)
		*ppv = static_cast<IDivision *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}
	
	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CMultiplicationDivision::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CMultiplicationDivision::Release(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}
	return(m_cRef);
}

// CMultiplicationDivision's IMultiplication specific method
HRESULT CMultiplicationDivision::MultiplicationOfTwoIntegers(int num1, int num2, int *pMultiplication)
{
	*pMultiplication = num1 * num2;
	return(S_OK);
}

// CMultiplicationDivision's IDivision specific method
HRESULT CMultiplicationDivision::DivisionOfTwoIntegers(int num1, int num2, int *pDivision)
{
	*pDivision = num1 / num2;
	return(S_OK);
}

// CMultiplcationDivisionClassFactory's method implementation
CMultiplicationDivisionClassFactory::CMultiplicationDivisionClassFactory(void)
{
	m_cRef = 1;
}

CMultiplicationDivisionClassFactory::~CMultiplicationDivisionClassFactory(void)
{

}

// CMultiplicationDivisionClassFactory's IUnknown specific methods
HRESULT CMultiplicationDivisionClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CMultiplicationDivisionClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CMultiplicationDivisionClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}
	return(m_cRef);
}

// CMultiplicationDivisionClassFactory's IClassFactory specific methods
HRESULT CMultiplicationDivisionClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// variables
	CMultiplicationDivision *pCMultiplicationDivision = NULL;
	HRESULT hr = S_OK;

	// code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	// create instance of the component 
	pCMultiplicationDivision = new CMultiplicationDivision;
	if (pCMultiplicationDivision == NULL)
		return(E_OUTOFMEMORY);

	// get the requested interface
	hr = pCMultiplicationDivision->QueryInterface(riid, ppv);
	pCMultiplicationDivision->Release();
	return(hr);
}

HRESULT CMultiplicationDivisionClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

// Implementation of Exported functions of DLL
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variables
	CMultiplicationDivisionClassFactory *pCMultiplicationDivisionClassFactory = NULL;
	HRESULT hr;

	// code
	if (rclsid != CLSID_MultiplicationDivision)
		return(CLASS_E_CLASSNOTAVAILABLE);

	// create class factory object
	pCMultiplicationDivisionClassFactory = new CMultiplicationDivisionClassFactory;
	if (pCMultiplicationDivisionClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pCMultiplicationDivisionClassFactory->QueryInterface(riid, ppv);
	pCMultiplicationDivisionClassFactory->Release();
	return(hr);
}

HRESULT __stdcall DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}
/ Implementation of CSumSubtract's constructor method
CSumSubtract::CSumSubtract(void)
{
	// code
	m_cRef = 1; // hardcoded initialization to anticipate possible failure of QueryInterface
	InterlockedIncrement(&glNumberOfActiveComponents); // Increment global counter
}

// Implementation of CSumSubtract's destructor method
CSumSubtract::~CSumSubtract(void)
{
	// code
	InterlockedDecrement(&glNumberOfActiveComponents); // decrement global counter
}

// Implementation of CSumSubtract's IUnknown's methods
HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
	// code
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
	// code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CSumSubtract::Release(void)
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

// Implementation of ISum's Methods
HRESULT CSumSubtract::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
	// code
	*pSum = num1 + num2;
	return(S_OK);
}

// Implementation of ISubtract's Method
HRESULT CSumSubtract::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
	// code
	*pSubtract = num1 - num2;
	return(S_OK);
}

// Implementation of CSumSubtractClassFactory's Constructor Methods
CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
	// code
	m_cRef = 1; // hardcoded initialization to anticipate failure of QueryInterface()
}

// Implementation of CSumSubtractClassFactory's Destructor Methods
CSumSubtractClassFactory::~CSumSubtractClassFactory(void)
{
	// code
}

// Implementation of CSumSubtractClassFactory's IClassFactory's IUnknown's Methods
HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void ** ppv)
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

ULONG CSumSubtractClassFactory::AddRef(void)
{
	// code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CSumSubtractClassFactory::Release(void)
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

// Implementation of CSumSubtract's IClassFactory's Methods
HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// variable declaration
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr;

	// code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	// create the instance of component i.e. of CSumSubtract class
	pCSumSubtract = new CSumSubtract;
	if (pCSumSubtract == NULL)
		return(E_OUTOFMEMORY);
	
	// get the requested interface
	hr = pCSumSubtract->QueryInterface(riid, ppv);
	pCSumSubtract->Release(); // anticipate possible failure of QueryInterface()
	return(hr);
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	// code
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

// Implementation of exported functions from this DLL
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variable declaration
	CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
	HRESULT hr;

	// code
	if (rclsid != CLSID_SumSubtract)
		return(CLASS_E_CLASSNOTAVAILABLE);

	// create class factory
	pCSumSubtractClassFactory = new CSumSubtractClassFactory;
	if (pCSumSubtractClassFactory == NULL)
		return(E_OUTOFMEMORY);
	hr = pCSumSubtractClassFactory->QueryInterface(riid, ppv);
	pCSumSubtractClassFactory->Release(); // anticipate possible failure of QueryInterface()
	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	// code
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}


