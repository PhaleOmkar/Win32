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

// CLSID of SumSubtract Component {57218C15-150A-4AE1-9A7B-AF4DEE5A6E10}
const CLSID CLSID_SumSubtract = { 0x57218c15, 0x150a, 0x4ae1, 0x9a, 0x7b, 0xaf, 0x4d, 0xee, 0x5a, 0x6e, 0x10 };

// IID of ISum Interface {02770F33-DA67-408B-B93B-F877DE39CCCE}
const IID IID_ISum = { 0x2770f33, 0xda67, 0x408b, 0xb9, 0x3b, 0xf8, 0x77, 0xde, 0x39, 0xcc, 0xce };

// IID of ISubtract Interface {EB3E4392-D735-45E2-BDD1-75DDA953B6F0}
const IID IID_ISubtract = { 0xeb3e4392, 0xd735, 0x45e2, 0xbd, 0xd1, 0x75, 0xdd, 0xa9, 0x53, 0xb6, 0xf0 };


// IID of IMultiplication Interface {0618501D-EA26-42C5-80E0-1AF8ABCEED4B}
const IID IID_IMultiplication = { 0x618501d, 0xea26, 0x42c5, 0x80, 0xe0, 0x1a, 0xf8, 0xab, 0xce, 0xed, 0x4b };

// IID of IDivision Interface {6F16B883-FC5E-4D65-8E49-A0CF73C38175}
const IID IID_IDivision = { 0x6f16b883, 0xfc5e, 0x4d65, 0x8e, 0x49, 0xa0, 0xcf, 0x73, 0xc3, 0x81, 0x75 };