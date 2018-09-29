class ISum : public IUnknown
{
public:
	// Implementation of ISum specific methods
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;
};

class ISubtract : public IUnknown
{
public:
	// Implementation of ISubtract specific methods
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
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

// CLSID of SumSubtract Component {BA362BB2-B8B4-458A-B6F6-924A6F688681}
const CLSID CLSID_SumSubtract = { 0xba362bb2, 0xb8b4, 0x458a, 0xb6, 0xf6, 0x92, 0x4a, 0x6f, 0x68, 0x86, 0x81 };

// IID of ISum Interface {53181514-B398-4513-804A-9D96F242075F}
const IID IID_ISum = { 0x53181514, 0xb398, 0x4513, 0x80, 0x4a, 0x9d, 0x96, 0xf2, 0x42, 0x7, 0x5f };

// IID of ISubtract Interface {54C011CD-6F9B-4957-8775-E40269145F8E}
const IID IID_ISubtract = { 0x54c011cd, 0x6f9b, 0x4957, 0x87, 0x75, 0xe4, 0x2, 0x69, 0x14, 0x5f, 0x8e };

// IID of IMultiplication Interface {723A55CC-C3FB-4A9B-B9FF-85BE20794B54}
const IID IID_IMultiplication = { 0x723a55cc, 0xc3fb, 0x4a9b, 0xb9, 0xff, 0x85, 0xbe, 0x20, 0x79, 0x4b, 0x54 };

// IID of IDivision Interface {840988E3-E9B5-4888-86F4-6C01148ECA18}
const IID IID_IDivision = { 0x840988e3, 0xe9b5, 0x4888, 0x86, 0xf4, 0x6c, 0x1, 0x14, 0x8e, 0xca, 0x18 };