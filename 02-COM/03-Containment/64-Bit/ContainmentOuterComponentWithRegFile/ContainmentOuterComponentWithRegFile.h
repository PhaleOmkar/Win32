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

// CLSID of SumSubtract Component {F70B6A1F-B9C0-47BB-9EA4-346E89432138}
const CLSID CLSID_SumSubtract = { 0xf70b6a1f, 0xb9c0, 0x47bb, 0x9e, 0xa4, 0x34, 0x6e, 0x89, 0x43, 0x21, 0x38 };

// IID of ISum Interface {79760B0B-D66D-4B19-BD6D-54B3E589D1C3}
const IID IID_ISum = { 0x79760b0b, 0xd66d, 0x4b19, 0xbd, 0x6d, 0x54, 0xb3, 0xe5, 0x89, 0xd1, 0xc3 };

// IID of ISubtract Interface {32B0B3C7-C0F0-45D4-B5F8-FFA364B34AE1}
const IID IID_ISubtract = { 0x32b0b3c7, 0xc0f0, 0x45d4, 0xb5, 0xf8, 0xff, 0xa3, 0x64, 0xb3, 0x4a, 0xe1 };
