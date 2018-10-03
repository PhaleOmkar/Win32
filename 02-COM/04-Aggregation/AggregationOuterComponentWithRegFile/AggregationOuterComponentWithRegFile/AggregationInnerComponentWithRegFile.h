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

// CLSID of MultiplicationDivision Component {1F176428-9501-4BC9-BFD5-A622D7E860BC}
const CLSID CLSID_MultiplicationDivision = { 0x1f176428, 0x9501, 0x4bc9, 0xbf, 0xd5, 0xa6, 0x22, 0xd7, 0xe8, 0x60, 0xbc };

// IID of IMultiplication Interface {C7E06171-2210-4F56-99E0-C8D9C9097316}
const IID IID_IMultiplication = { 0xc7e06171, 0x2210, 0x4f56, 0x99, 0xe0, 0xc8, 0xd9, 0xc9, 0x9, 0x73, 0x16 };

// IID of IDivision Interface {FBA400D6-F0D5-4D1D-9AFB-78BB7AEF6F7E}
const IID IID_IDivision = { 0xfba400d6, 0xf0d5, 0x4d1d, 0x9a, 0xfb, 0x78, 0xbb, 0x7a, 0xef, 0x6f, 0x7e };
