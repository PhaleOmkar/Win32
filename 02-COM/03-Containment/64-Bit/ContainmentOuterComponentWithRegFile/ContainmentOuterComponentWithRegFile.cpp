#define UNICODE
#include <Windows.h>
#include "ContainmentInnerComponentWithRegFile.h"
#include "ContainmentOuterComponentWithRegFile.h"

// class declarations
class CSumSubtract : public ISum, ISubtract, IMultiplication, IDivision
{
private:
	long m_cRef;
	IMultiplication *pIMultiplication;
	IDivision *pIDivision;

public:
	CSumSubtract(void);
	~CSumSubtract(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISum specific method
	HRESULT __stdcall SumOfTwoIntegers(int, int, int *);

	// ISubtract specific method
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *);

	// IMultiplication specific method
	HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int *);

	// IDivision specific methos
	HRESULT __stdcall DivisionOfTwoIntegers(int, int, int *);

	// custom method from Inner component creation
	HRESULT __stdcall InitializeInnerComponents(void);
};

class CSumSubtractClassFactory : public IClassFactory
{
private:
	long m_cRef;

public: 
	CSumSubtractClassFactory(void);
	~CSumSubtractClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

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

// Implementation of CSumSubtract's constructor method
CSumSubtract::CSumSubtract(void)
{
	// code
	// initialize private data members
	pIMultiplication = NULL;
	pIDivision = NULL;
	m_cRef = 1; 
	InterlockedIncrement(&glNumberOfActiveComponents); 
}

// Implementation of CSumSubtract's destructor method
CSumSubtract::~CSumSubtract(void)
{
	// code
	InterlockedDecrement(&glNumberOfActiveComponents); 
	if (pIMultiplication)
	{
		pIMultiplication->Release();
		pIMultiplication = NULL;
	}
	if (pIDivision)
	{
		pIDivision->Release();
		pIDivision = NULL;
	}
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

// Implementation of IMultiplication's Methods
HRESULT CSumSubtract::MultiplicationOfTwoIntegers(int num1, int num2, int *pMultiplication)
{
	// code
	// delegate to inner component
	pIMultiplication->MultiplicationOfTwoIntegers(num1, num2, pMultiplication);
	return(S_OK);
}

// Implementation of IDivision's Method
HRESULT CSumSubtract::DivisionOfTwoIntegers(int num1, int num2, int *pDivision)
{
	// code
	// delegate to inner component
	pIDivision->DivisionOfTwoIntegers(num1, num2, pDivision);
	return(S_OK);
}

HRESULT CSumSubtract::InitializeInnerComponents(void)
{
	// variable declaration
	HRESULT hr = S_OK;

	// code
	hr = CoCreateInstance(CLSID_MultiplicationDivision, NULL, CLSCTX_INPROC_SERVER, IID_IMultiplication, (void **)&pIMultiplication);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("IMultiplication Interface Cannot Be Obtained From Inner Component."), TEXT("ERROR"), MB_OK);
		return(E_FAIL);
	}

	pIMultiplication->QueryInterface(IID_IDivision, (void **)&pIDivision);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("IDivision Interface Cannot Be Obtained From Inner Component."), TEXT("Error"), MB_OK);
		return(E_FAIL);
	}
	return(S_OK);
}

// Implementation of CSumSubtractClassFactory's Constructor Methods
CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
	// code
	m_cRef = 1; 
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

	// initialize the Inner Components
	hr = pCSumSubtract->InitializeInnerComponents();
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed To Initialize Inner Components."), TEXT("Error"), MB_OK);
		pCSumSubtract->Release();
		return(hr);
	}

	// get the requested interface
	hr = pCSumSubtract->QueryInterface(riid, ppv);
	pCSumSubtract->Release(); 
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
	pCSumSubtractClassFactory->Release(); 
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
