#define UNICODE
#include <Windows.h>
#include "ChemistryDll.h"

// Class declaration
class CChemistry : public IChemisty
{
private:
	long m_cRef;

public:
	// constructor method declaration
	CChemistry(void);
	// destructor method declaration
	~CChemistry(void);

	// IUnknown specific method declarations (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IChemistry specific method declaration (inherited)
	HRESULT __stdcall CalculateNumberOfMolecules(double, double, double *);
	HRESULT __stdcall CalculateNumberOfAtoms(double, double, int, double *);
};

class CChemistryClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	// Constructor method declaration
	CChemistryClassFactory(void);
	// Destructor method declaration
	~CChemistryClassFactory(void);

	// IUnknown specific method declaration (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific method declaration (inherited)
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// global variables
long glNumberOfActiveComponents = 0;
long glNumerOfServerLocks = 0;

// DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
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

	return TRUE;
}

// Implementation of CChemistry methods
CChemistry::CChemistry(void)
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CChemistry::~CChemistry(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

// Implementation of CChemistry's IUnknown Interface
HRESULT CChemistry::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IChemisty *>(this);
	else if (riid == IID_IChemistry)
		*ppv = static_cast<IChemisty *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CChemistry::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CChemistry::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}

	return(m_cRef);
}

// Implementation of CChemisty's IChemistry Interface
HRESULT CChemistry::CalculateNumberOfMolecules(double dMolecularMass, double dMassInGrams, double *pNumberOfMolecules)
{
	*pNumberOfMolecules = ((6.022E+23) / dMolecularMass) * dMassInGrams;
	return(S_OK);
}

HRESULT CChemistry::CalculateNumberOfAtoms(double dMolecularMass, double dMassInGrams, int iNoOfAtomsInMolecule, double *pNumberOfAtoms)
{
	double dNumberOfMolecules = 0.0;
	CalculateNumberOfMolecules(dMolecularMass, dMassInGrams, &dNumberOfMolecules);
	*pNumberOfAtoms =  dNumberOfMolecules * iNoOfAtomsInMolecule;
		
	return(S_OK);
}


// Implementation of CChemistryClassFactory methods
CChemistryClassFactory::CChemistryClassFactory(void)
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CChemistryClassFactory::~CChemistryClassFactory(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

// Implementation of CChemistryClassFactory's IUnknown Interface Methods
HRESULT CChemistryClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CChemistryClassFactory::AddRef(void)
{
	// code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CChemistryClassFactory::Release(void)
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

// Implementation of CChemistryClassFactory's IClassFactory Interface Methods
HRESULT CChemistryClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// variable declaration
	CChemistry *pCChemistry = NULL;
	HRESULT hr;

	// code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	// create the instance of component 
	pCChemistry = new CChemistry;
	if (pCChemistry == NULL)
		return(E_OUTOFMEMORY);

	// get the requested interface
	hr = pCChemistry->QueryInterface(riid, ppv);
	pCChemistry->Release(); // anticipate possible failure of QueryInterface()
	return(hr);
}

HRESULT CChemistryClassFactory::LockServer(BOOL fLock)
{
	// code
	if (fLock)
		InterlockedIncrement(&glNumerOfServerLocks);
	else
		InterlockedDecrement(&glNumerOfServerLocks);
	return(S_OK);
}

// Implementation of exported functions in DLL
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variable declaration
	CChemistryClassFactory *pCChemistryClassFactory = NULL;
	HRESULT hr = S_OK;

	// code
	if (rclsid != CLSID_Chemistry)
		return(CLASS_E_CLASSNOTAVAILABLE);

	// create class factory
	pCChemistryClassFactory = new CChemistryClassFactory;
	if (pCChemistryClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pCChemistryClassFactory->QueryInterface(riid, ppv);
	pCChemistryClassFactory->Release();
	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	// code
	if ((glNumberOfActiveComponents == 0) && (glNumerOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}
