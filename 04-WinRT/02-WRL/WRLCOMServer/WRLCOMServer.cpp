#include "pch.h"

#include "WRLCOMServer_h.h"
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;

namespace ABI
{
	namespace WRLCOMServer
	{
		class CSumSubtract: public RuntimeClass<ISum, ISubtract>
		{
			InspectableClass(L"WRLCOMServer.CSumSubtract", BaseTrust)

			public:
			CSumSubtract()
			{
			}

			HRESULT __stdcall SumOfTwoIntegers(int a, int b, int* value)
			{
				*value = a + b;
				return S_OK;
			}
			
			HRESULT __stdcall SubtractionOfTwoIntegers(int a, int b, int* value)
			{
				*value = a - b;
				return S_OK;
			}

		};

		ActivatableClass(CSumSubtract);
	}
}