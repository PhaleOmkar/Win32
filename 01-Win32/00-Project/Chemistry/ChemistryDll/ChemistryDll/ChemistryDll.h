class IChemisty : public IUnknown
{
public:
	virtual HRESULT __stdcall CalculateNumberOfMolecules(double, double, double*) = 0;
	virtual HRESULT __stdcall CalculateNumberOfAtoms(double, double, int, double*) = 0;
};

// CLSID of Chemistry Component {AE495E66-237D-404E-99D4-7663BD8D2ABC}
const CLSID CLSID_Chemistry = { 0xae495e66, 0x237d, 0x404e, 0x99, 0xd4, 0x76, 0x63, 0xbd, 0x8d, 0x2a, 0xbc };

// IID of IChemistry Interface {E0B150EC-252C-44EB-9258-C37297597DCE}
const IID IID_IChemistry = { 0xe0b150ec, 0x252c, 0x44eb, 0x92, 0x58, 0xc3, 0x72, 0x97, 0x59, 0x7d, 0xce };
