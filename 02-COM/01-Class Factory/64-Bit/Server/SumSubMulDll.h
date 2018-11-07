class ISum : public IUnknown
{
public:
	// ISum specific method declaration
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;
};

class ISubtract : public IUnknown
{
public:
	// ISubtract specific method declaration
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;
};

class IMultiply : public IUnknown
{
public:
	// IMultiply specific method declaration
	virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int *) = 0;
};

// CLSID of SumSubMul component {3274F6C2-B31C-4A75-91BD-E583B3222C88}
const CLSID CLSID_SumSubMul = { 0x3274f6c2, 0xb31c, 0x4a75, 0x91, 0xbd, 0xe5, 0x83, 0xb3, 0x22, 0x2c, 0x88 };

// IID of ISum Interface {EFA86214-E8FA-4550-A208-EF3F490583FA}
const IID IID_ISum = { 0xefa86214, 0xe8fa, 0x4550, 0xa2, 0x8, 0xef, 0x3f, 0x49, 0x5, 0x83, 0xfa };

// IID of Isubtract interface {E7AF6A25-4EEA-47BF-853D-2EBA4D3243E1}
const IID IID_ISubtract = { 0xe7af6a25, 0x4eea, 0x47bf, 0x85, 0x3d, 0x2e, 0xba, 0x4d, 0x32, 0x43, 0xe1 };

// IID of IMultiply interface {3315876A-0730-48CC-BF29-2D0E9B9BDFDA}
const IID IID_IMultiply = { 0x3315876a, 0x730, 0x48cc, 0xbf, 0x29, 0x2d, 0xe, 0x9b, 0x9b, 0xdf, 0xda };

