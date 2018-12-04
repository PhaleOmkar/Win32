//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace HelloWorldNoGridWithNamespaces;

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

using namespace Windows::UI;
using namespace Windows::UI::Text;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();

	Grid^ grid = ref new Grid();
	grid->Background = ref new SolidColorBrush(Colors::Black);

	TextBlock^ textBlock = ref new TextBlock();
	textBlock->Text = "Hello World!";
	textBlock->FontFamily = ref new Media::FontFamily("Segoe UI");
	textBlock->FontSize = 120;
	textBlock->FontStyle = Text::FontStyle::Oblique;
	textBlock->FontWeight = FontWeights::Bold;
	textBlock->Foreground = ref new SolidColorBrush(Colors::Gold);
	textBlock->HorizontalAlignment = Xaml::HorizontalAlignment::Center;
	textBlock->VerticalAlignment = Xaml::VerticalAlignment::Center;

	grid->Children->Append(textBlock);
	this->Content = grid;


}
