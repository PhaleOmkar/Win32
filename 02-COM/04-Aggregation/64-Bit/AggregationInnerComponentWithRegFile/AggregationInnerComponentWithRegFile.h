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

// CLSID of MultiplicationDivision Component {AD1D4D1B-C0C8-49F1-8D55-37B82EAA393D}
const CLSID CLSID_MultiplicationDivision = { 0xad1d4d1b, 0xc0c8, 0x49f1, 0x8d, 0x55, 0x37, 0xb8, 0x2e, 0xaa, 0x39, 0x3d };

// IID of IMultiplication Interface {0618501D-EA26-42C5-80E0-1AF8ABCEED4B}
const IID IID_IMultiplication = { 0x618501d, 0xea26, 0x42c5, 0x80, 0xe0, 0x1a, 0xf8, 0xab, 0xce, 0xed, 0x4b };

// IID of IDivision Interface {6F16B883-FC5E-4D65-8E49-A0CF73C38175}
const IID IID_IDivision = { 0x6f16b883, 0xfc5e, 0x4d65, 0x8e, 0x49, 0xa0, 0xcf, 0x73, 0xc3, 0x81, 0x75 };
