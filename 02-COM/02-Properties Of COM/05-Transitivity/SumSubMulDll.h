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

// CLSID of SumSubMul component {087F04DE-4C85-4E48-A0F3-2945D60C67A3}
const CLSID CLSID_SumSubMul = { 0x87f04de, 0x4c85, 0x4e48, 0xa0, 0xf3, 0x29, 0x45, 0xd6, 0xc, 0x67, 0xa3 };

// IID of ISum Interface {B6F2BC4F-B82E-4127-905A-30816D67F264}
const IID IID_ISum = { 0xb6f2bc4f, 0xb82e, 0x4127, 0x90, 0x5a, 0x30, 0x81, 0x6d, 0x67, 0xf2, 0x64 };

// IID of Isubtract interface {9D393859-9081-4FFE-8C0E-249861E49D23}
const IID IID_ISubtract = { 0x9d393859, 0x9081, 0x4ffe, 0x8c, 0xe, 0x24, 0x98, 0x61, 0xe4, 0x9d, 0x23 };

// IID of IMultiply interface {E34FD0B8-DE76-4886-96D0-03EF900687D5}
const IID IID_IMultiply = { 0xe34fd0b8, 0xde76, 0x4886, 0x96, 0xd0, 0x3, 0xef, 0x90, 0x6, 0x87, 0xd5 };

