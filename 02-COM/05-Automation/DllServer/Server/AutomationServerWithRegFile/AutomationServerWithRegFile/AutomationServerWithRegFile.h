#pragma once

class IMyMath : public IDispatch
{
public:
	//pure virtual methods
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;

	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;
};

// CLSID of MyMath Component {43BE8D41-2BAD-48F8-AB87-12F4A2CCB4D4}
const CLSID CLSID_MyMath = { 0x43be8d41, 0x2bad, 0x48f8, 0xab, 0x87, 0x12, 0xf4, 0xa2, 0xcc, 0xb4, 0xd4 };

// IID of IMyMath interface {809A2015-4D22-4814-9DC4-26211829C27A}
const IID IID_IMyMath = { 0x809a2015, 0x4d22, 0x4814, 0x9d, 0xc4, 0x26, 0x21, 0x18, 0x29, 0xc2, 0x7a };
