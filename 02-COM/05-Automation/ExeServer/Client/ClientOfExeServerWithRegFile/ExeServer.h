#pragma once

class ISum : public IUnknown
{
public:
	virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;
};

class ISubtract : public IUnknown
{
public:
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;
};

// CLSID of CSumSubtract class {852827CB-15AC-4D20-96D8-8DDBAC886F24}
const CLSID CLSID_CSumSubtract = { 0x852827cb, 0x15ac, 0x4d20, 0x96, 0xd8, 0x8d, 0xdb, 0xac, 0x88, 0x6f, 0x24 };

// IID of ISum interface {0703F26A-B6AC-4E50-983E-275AC15D2DF0}
const IID IID_ISum = { 0x703f26a, 0xb6ac, 0x4e50, 0x98, 0x3e, 0x27, 0x5a, 0xc1, 0x5d, 0x2d, 0xf0 };

// IID of ISubtract interface {501D0F4A-8303-4174-90A8-4CCEC057CFE9}
const IID IID_ISubtract = { 0x501d0f4a, 0x8303, 0x4174, 0x90, 0xa8, 0x4c, 0xce, 0xc0, 0x57, 0xcf, 0xe9 };


