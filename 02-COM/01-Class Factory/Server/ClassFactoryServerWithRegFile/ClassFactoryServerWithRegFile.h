class ISum : public IUnknown
{
	// ISum specific method declaration
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;
};

class ISubtract : public IUnknown
{
	// ISubtract specific method declaration
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;
};

// CLSID of SumSubtract component {907180A7-1DCB-492C-8316-29C6127D7E66}
const CLSID CLSID_SumSubtract = { 0x907180a7, 0x1dcb, 0x492c, 0x83, 0x16, 0x29, 0xc6, 0x12, 0x7d, 0x7e, 0x66 };

// IID of ISum Interface {B6F2BC4F-B82E-4127-905A-30816D67F264}
const IID IID_ISum = { 0xb6f2bc4f, 0xb82e, 0x4127, 0x90, 0x5a, 0x30, 0x81, 0x6d, 0x67, 0xf2, 0x64 };

// IID of Isubtract interface {9D393859-9081-4FFE-8C0E-249861E49D23}
const IID IID_ISubtract = { 0x9d393859, 0x9081, 0x4ffe, 0x8c, 0xe, 0x24, 0x98, 0x61, 0xe4, 0x9d, 0x23 };