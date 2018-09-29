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

// IID of ISum Interface {7D538C67-3D74-40D3-924C-C575D88AFD3D}
const IID IID_ISum = { 0x7d538c67, 0x3d74, 0x40d3, 0x92, 0x4c, 0xc5, 0x75, 0xd8, 0x8a, 0xfd, 0x3d };

// IID of Isubtract interface {F4ACBCFD-1754-4E24-B0AF-A782EBD57E9B}
const IID IID_ISubtract = { 0xf4acbcfd, 0x1754, 0x4e24, 0xb0, 0xaf, 0xa7, 0x82, 0xeb, 0xd5, 0x7e, 0x9b };

// IID of IMultiply interface {E34FD0B8-DE76-4886-96D0-03EF900687D5}
const IID IID_IMultiply = { 0xe34fd0b8, 0xde76, 0x4886, 0x96, 0xd0, 0x3, 0xef, 0x90, 0x6, 0x87, 0xd5 };

