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

// CLSID of SumSubtract Component {F70B6A1F-B9C0-47BB-9EA4-346E89432138}
const CLSID CLSID_SumSubtract = { 0xf70b6a1f, 0xb9c0, 0x47bb, 0x9e, 0xa4, 0x34, 0x6e, 0x89, 0x43, 0x21, 0x38 };

// IID of ISum Interface {79760B0B-D66D-4B19-BD6D-54B3E589D1C3}
const IID IID_ISum = { 0x79760b0b, 0xd66d, 0x4b19, 0xbd, 0x6d, 0x54, 0xb3, 0xe5, 0x89, 0xd1, 0xc3 };

// IID of ISubtract Interface {32B0B3C7-C0F0-45D4-B5F8-FFA364B34AE1}
const IID IID_ISubtract = { 0x32b0b3c7, 0xc0f0, 0x45d4, 0xb5, 0xf8, 0xff, 0xa3, 0x64, 0xb3, 0x4a, 0xe1 };

// IID of IMultiplication Interface {02FBA2BC-621B-4CA5-9024-A5C3937C3406}
const IID IID_IMultiplication = { 0x2fba2bc, 0x621b, 0x4ca5, 0x90, 0x24, 0xa5, 0xc3, 0x93, 0x7c, 0x34, 0x6 };

// IID of IDivision Interface {FF8047B1-63B5-4E0D-8427-D23C05AF7E24}
const IID IID_IDivision = { 0xff8047b1, 0x63b5, 0x4e0d, 0x84, 0x27, 0xd2, 0x3c, 0x5, 0xaf, 0x7e, 0x24 };