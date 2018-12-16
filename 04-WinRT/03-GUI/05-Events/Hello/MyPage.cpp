#include "pch.h"
#include "MyPage.h"

using namespace Windows::UI;
using namespace Windows::UI::Text;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Controls;

MyPage::MyPage()
{

	Window::Current->CoreWindow->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MyPage::OnKeyDown);

	Grid^ grid = ref new Grid();

	textBlock = ref new TextBlock();
	textBlock->Text = "Hello World!";
	textBlock->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe UI");
	textBlock->FontSize = 50;
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
	button->Tapped += ref new TappedEventHandler(this, &MyPage::OnTapped);
	button->RightTapped += ref new RightTappedEventHandler(this, &MyPage::OnRightTapped);
	button->DoubleTapped += ref new Windows::UI::Xaml::Input::DoubleTappedEventHandler(this, &MyPage::OnDoubleTapped);
	button->Holding += ref new Windows::UI::Xaml::Input::HoldingEventHandler(this, &MyPage::OnHolding);

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

void MyPage::OnTapped(Object^ sender, TappedRoutedEventArgs^ args)
{
	textBlock->Text = "Tapped! or Clicked!";
}

void MyPage::OnRightTapped(Object^ sender, RightTappedRoutedEventArgs^ args)
{
	textBlock->Text = "Right Tapped! or Clicked!";
}


void MyPage::OnDoubleTapped(Object^ sender, DoubleTappedRoutedEventArgs^ args)
{
	textBlock->Text = "Double Tapped! or Clicked!";
}

void MyPage::OnHolding(Object^ sender, HoldingRoutedEventArgs^ args)
{
	textBlock->Text = "Holding!";
}
