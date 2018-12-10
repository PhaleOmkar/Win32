#include "pch.h"
#include "MyPage.h"

using namespace Windows::UI;
using namespace Windows::UI::Text;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Controls;

MyPage::MyPage()
{

	Grid^ grid = ref new Grid();

	textBlock = ref new TextBlock();
	textBlock->Text = "Hello World!";
	textBlock->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe UI");
	textBlock->FontSize = 100;
	textBlock->FontStyle = Windows::UI::Text::FontStyle::Oblique;
	textBlock->FontWeight = Windows::UI::Text::FontWeights::Bold;
	textBlock->Foreground = ref new SolidColorBrush(Windows::UI::Colors::Gold);
	textBlock->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
	textBlock->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;

	grid->Children->Append(textBlock);

	Button^ button = ref new Button();
	button->Content = "Click Me!";
	button->Width = 400;
	button->Height = 150;
	button->BorderThickness = 12;
	button->BorderBrush = ref new SolidColorBrush(Colors::Gold);
	button->Foreground = ref new SolidColorBrush(Colors::Red);
	button->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe UI");
	button->FontSize = 50;
	button->FontWeight = Windows::UI::Text::FontWeights::Bold;
	button->FontStyle = Windows::UI::Text::FontStyle::Oblique;
	button->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
	button->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom;
	button->Click += ref new RoutedEventHandler(this, &MyPage::OnButtonClick);

	grid->Children->Append(button);
	this->Content = grid;

}

void MyPage::OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
{
	if(args->VirtualKey == VirtualKey::Space)
		textBlock->Text = "Space Pressed!";
	else
		textBlock->Text = "Key Pressed!";

}

void MyPage::OnButtonClick(Object^ sender, RoutedEventArgs^ args)
{
	textBlock->Text = "Button Clicked!";
}


