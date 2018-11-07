#define UNICODE
#include <Windows.h>
#include "AggregationInnerComponentWithRegFile.h"
#include "AggregationOuterComponentWithRegFile.h"

// class declaration
class CSumSubtract : public ISum, ISubtract
{
private:
	long m_cRef;
	IUnknown *m_pIUnknownInner;
	IMultiplication *m_pIMultiplication;
	IDivision *m_pIDivision;

public: 
	CSumSubtract(void);
	~CSumSubtract(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISum specific methods
	HRESULT __stdcall SumOfTwoIntegers(int, int, int *);

	// ISubtract specific methods
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *);

	HRESULT __stdcall InitializeInnerComponent(void);
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

	// IClassFactory specific method declaration
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void **);
	HRESULT __stdcall LockServer(BOOL);
};

// Global variables
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

// DllMain
BOOL WINAPI DllMain(HINSTANCE hInstace, DWORD dwReason, LPVOID Reserved)
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

// Implementation of CSumSubtract's constructor methods
CSumSubtract::CSumSubtract(void)
{
	// code
	// initialization of private data
	m_pIUnknownInner = NULL;
	m_pIMultiplication = NULL;
	m_pIDivision = NULL;
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract(void)
{
	// code
	InterlockedDecrement(&glNumberOfActiveComponents);
	if (m_pIMultiplication)
	{
		m_pIMultiplication->Release();
		m_pIMultiplication = NULL;
	}
	if (m_pIDivision)
	{
		m_pIDivision->Release();
		m_pIDivision = NULL;
	}
	if (m_pIUnknownInner)
	{
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
	}
}

// Implementation of CSumSubtract's IUnknown methods
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
		return(m_pIUnknownInner->QueryInterface(riid, ppv));
	else if (riid == IID_IDivision)
		return(m_pIUnknownInner->QueryInterface(riid, ppv));
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(this)->AddRef();
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
		return(0);
	}
	return(m_cRef);
}

// Implementation of ISum's methods
HRESULT CSumSubtract::SumOfTwoIntegers(int iNum1, int iNum2, int *pResult)
{
	// code
	*pResult = iNum1 + iNum2;
	return(S_OK);
}

// Implementation of ISubtract's methods
HRESULT CSumSubtract::SubtractionOfTwoIntegers(int iNum1, int iNum2, int *pResult)
{
	// code
	*pResult = iNum1 - iNum2;
	return(S_OK);
}

HRESULT CSumSubtract::InitializeInnerComponent(void)
{
	// variable declaration
	HRESULT hr;

	// code
	hr = CoCreateInstance(CLSID_MultiplicationDivision, reinterpret_cast<IUnknown *>(this), CLSCTX_INPROC_SERVER, 
		IID_IUnknown, (void **)&m_pIUnknownInner);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("IUnknown interface cannot be obtained from Inner component"), TEXT("Error"), MB_OK);
		return(E_FAIL);
	}

	hr = m_pIUnknownInner->QueryInterface(IID_IMultiplication, (void **)&m_pIMultiplication);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("IMultiplication interface cannot be obtained from Inner compnent"), TEXT("Error"), MB_OK);
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
	}

	hr = m_pIUnknownInner->QueryInterface(IID_IDivision, (void **)&m_pIDivision);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("IDivision interface cannot be obtained from Inner compnent"), TEXT("Error"), MB_OK);
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
	}

	return(S_OK);
}

// Implementation of CSumSubtractClassFactory's methods
CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
	m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory(void)
{

}

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

	reinterpret_cast<IUnknown *>(this)->AddRef();
	return(S_OK);
}

ULONG CSumSubtractClassFactory::AddRef(void)
{
	// code
	InterlockedIncrement(&glNumberOfActiveComponents);
	return(glNumberOfActiveComponents);
}

ULONG CSumSubtractClassFactory::Release(void)
{
	// code
	InterlockedDecrement(&glNumberOfActiveComponents);
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
	hr = pCSumSubtract->InitializeInnerComponent();
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





















