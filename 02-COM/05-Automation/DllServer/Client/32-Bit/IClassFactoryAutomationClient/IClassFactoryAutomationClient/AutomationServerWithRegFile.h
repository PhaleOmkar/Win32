#pragma once

class IMyMath : public IDispatch
{
public:
	//pure virtual methods
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;

	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;
};

// CLSID of MyMath Component {8C321D8D-9289-447E-BBF3-6F58A6F6CB3E}
const CLSID CLSID_MyMath = { 0x8c321d8d, 0x9289, 0x447e, 0xbb, 0xf3, 0x6f, 0x58, 0xa6, 0xf6, 0xcb, 0x3e };

// IID of IMyMath interface {A1E4445F-9268-4FBC-86D3-2C699131B98F}
const IID IID_IMyMath = { 0xa1e4445f, 0x9268, 0x4fbc, 0x86, 0xd3, 0x2c, 0x69, 0x91, 0x31, 0xb9, 0x8f };
