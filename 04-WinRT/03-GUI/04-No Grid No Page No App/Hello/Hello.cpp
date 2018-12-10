#include "pch.h"
#include "Hello.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Text;

void WndCallback(Windows::UI::Xaml::ApplicationInitializationCallbackParams^ params)
{
	App^ app = ref new App();
}

int main(Array<String^>^ args)
{
	ApplicationInitializationCallback^ callback = ref new ApplicationInitializationCallback(WndCallback);
	Application::Start(callback);
	return(0);
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args)
{
	TextBlock^ textBlock = ref new TextBlock();
	textBlock->Text = "Hello World!";
	textBlock->FontFamily = ref new FontFamily("Segoe UI");
	textBlock->FontSize = 120;
	textBlock->FontStyle = FontStyle::Oblique;
	textBlock->FontWeight = FontWeights::Bold;
	textBlock->Foreground = ref new SolidColorBrush(Windows::UI::Colors::Gold);
	textBlock->HorizontalAlignment = HorizontalAlignment::Center;
	textBlock->VerticalAlignment = VerticalAlignment::Center;
		
	Window::Current->Content = textBlock;
	Window::Current->Activate();

}
