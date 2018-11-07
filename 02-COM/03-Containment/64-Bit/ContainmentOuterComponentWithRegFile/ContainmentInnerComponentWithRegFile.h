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

// CLSID of MultiplicationDivision Component {5DA088EC-840D-40BC-A55F-E41401E1B8C8}
const CLSID CLSID_MultiplicationDivision = { 0x5da088ec, 0x840d, 0x40bc, 0xa5, 0x5f, 0xe4, 0x14, 0x1, 0xe1, 0xb8, 0xc8 };

// IID of IMultiplication Interface {02FBA2BC-621B-4CA5-9024-A5C3937C3406}
const IID IID_IMultiplication = { 0x2fba2bc, 0x621b, 0x4ca5, 0x90, 0x24, 0xa5, 0xc3, 0x93, 0x7c, 0x34, 0x6 };

// IID of IDivision Interface {FF8047B1-63B5-4E0D-8427-D23C05AF7E24}
const IID IID_IDivision = { 0xff8047b1, 0x63b5, 0x4e0d, 0x84, 0x27, 0xd2, 0x3c, 0x5, 0xaf, 0x7e, 0x24 };
