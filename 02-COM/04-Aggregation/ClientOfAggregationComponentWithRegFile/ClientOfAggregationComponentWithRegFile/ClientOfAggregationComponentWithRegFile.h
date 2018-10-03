class ISum : public IUnknown
{
public:
	// Implementation of ISum specific methods
	virtual  HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;
};

class ISubtract : public IUnknown
{
public:
	// Implementation of ISubtract specific methods
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;
};

class IMultiplication : public IUnknown
{
public:
	// Implementation of IMultiplication specific methods
	virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int *) = 0;
};

class IDivision : public IUnknown
{
public:
	// Implementation of IDivision specific methods
	virtual HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*) = 0;
};

// CLSID of SumSubtract Component {40C22722-EF2C-4A67-8431-BD7833209137}
const CLSID CLSID_SumSubtract = { 0x40c22722, 0xef2c, 0x4a67, 0x84, 0x31, 0xbd, 0x78, 0x33, 0x20, 0x91, 0x37 };

// IID of ISum Interface {E01A1F35-EBA3-4133-9303-4ED8B6F21C08}
const IID IID_ISum = { 0xe01a1f35, 0xeba3, 0x4133, 0x93, 0x3, 0x4e, 0xd8, 0xb6, 0xf2, 0x1c, 0x8 };

// IID of ISubtract Interface {5CCD7D45-7CBC-4D78-A5A7-F4220858DDD2}
const IID IID_ISubtract = { 0x5ccd7d45, 0x7cbc, 0x4d78, 0xa5, 0xa7, 0xf4, 0x22, 0x8, 0x58, 0xdd, 0xd2 };


// IID of IMultiplication Interface {C7E06171-2210-4F56-99E0-C8D9C9097316}
const IID IID_IMultiplication = { 0xc7e06171, 0x2210, 0x4f56, 0x99, 0xe0, 0xc8, 0xd9, 0xc9, 0x9, 0x73, 0x16 };

// IID of IDivision Interface {FBA400D6-F0D5-4D1D-9AFB-78BB7AEF6F7E}
const IID IID_IDivision = { 0xfba400d6, 0xf0d5, 0x4d1d, 0x9a, 0xfb, 0x78, 0xbb, 0x7a, 0xef, 0x6f, 0x7e };
