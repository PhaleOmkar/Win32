#pragma once
class IOddNumber : public IUnknown
{
public:
	virtual HRESULT __stdcall GetNextOddNumber(int *) = 0;
};

class IOddNumberFactory : public IUnknown
{
public:
	virtual HRESULT __stdcall SetFirstOddNumber(int, IOddNumber **) = 0;
};

// CLSID of OddNumber Component {2C3BC35B-275E-4223-ACDC-1EC606B911FC}
const CLSID CLSID_OddNumber = { 0x2c3bc35b, 0x275e, 0x4223, 0xac, 0xdc, 0x1e, 0xc6, 0x6, 0xb9, 0x11, 0xfc };

// IID of IOddNumber interface {9F20F0B1-1FD0-43A8-8551-DDD3071B8349}
const IID IID_IOddNumber = { 0x9f20f0b1, 0x1fd0, 0x43a8, 0x85, 0x51, 0xdd, 0xd3, 0x7, 0x1b, 0x83, 0x49 };

// IID of IOddNumberFactory interface {33263B3C-79A0-40BB-9DC7-D775C4D7BAE1}
const IID IID_IOddNumberFactory = { 0x33263b3c, 0x79a0, 0x40bb, 0x9d, 0xc7, 0xd7, 0x75, 0xc4, 0xd7, 0xba, 0xe1 };

