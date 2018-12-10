//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

using namespace Windows::UI::Core;   // CoreWindow, KeyEventArgs
using namespace Windows::Foundation; // TypedEventArgs
using namespace Windows::System;     // Virtual Key
using namespace Windows::UI::Xaml;   // RoutedEventArgs
using namespace Windows::UI::Xaml::Controls;  // Page

namespace HelloWorld
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
		void OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args);
		void OnButtonClick(Object^ sender, RoutedEventArgs^ args);
	};
}
