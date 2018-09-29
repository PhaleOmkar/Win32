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

// CLSID of MultiplicationDivision Component {5D0F1DDC-D5C2-4485-91C2-BB4CC4B103DA}
const CLSID CLSID_MultiplicationDivision = { 0x5d0f1ddc, 0xd5c2, 0x4485, 0x91, 0xc2, 0xbb, 0x4c, 0xc4, 0xb1, 0x3, 0xda };

// IID of IMultiplication Interface {723A55CC-C3FB-4A9B-B9FF-85BE20794B54}
const IID IID_IMultiplication = { 0x723a55cc, 0xc3fb, 0x4a9b, 0xb9, 0xff, 0x85, 0xbe, 0x20, 0x79, 0x4b, 0x54 };

// IID of IDivision Interface {840988E3-E9B5-4888-86F4-6C01148ECA18}
const IID IID_IDivision = { 0x840988e3, 0xe9b5, 0x4888, 0x86, 0xf4, 0x6c, 0x1, 0x14, 0x8e, 0xca, 0x18 };
