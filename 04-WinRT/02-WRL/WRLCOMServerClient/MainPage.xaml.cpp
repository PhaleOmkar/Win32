//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <iostream>

using namespace WRLCOMServer;
using namespace WRLCOMServerClient;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();

	auto sumSubtract = ref new CSumSubtract();

	auto sumValue = sumSubtract->SumOfTwoIntegers(300, 200);
	auto subtractValue = sumSubtract->SubtractionOfTwoIntegers(300, 200);

	txtBlock1->Text = Platform::String::Concat(L"Sum Of Two Numbers: ", sumValue.ToString());
	txtBlock2->Text = Platform::String::Concat(L"Subtraction Of Two Numbers: ", subtractValue.ToString());

}
