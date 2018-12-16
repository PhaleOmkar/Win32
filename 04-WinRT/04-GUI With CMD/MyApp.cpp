using namespace Platform;
using namespace Windows::UI::Text;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;

ref class MyApp sealed : public Application
{
public:
	virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args) override;
};


int main(Array<String^>^ args)
{
	Application::Start(ref new ApplicationInitializationCallback(
		[](ApplicationInitializationCallbackParams^ param) {
			MyApp^ app = ref new MyApp();
		}
	));
	return(0);
}

void MyApp::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args)
{
	TextBlock^ textBlock = ref new TextBlock();
	textBlock->Text = "Hello World!";
	textBlock->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe UI");
	textBlock->FontSize = 50;
	textBlock->FontStyle = Windows::UI::Text::FontStyle::Oblique;
	textBlock->FontWeight = Windows::UI::Text::FontWeights::Bold;
	textBlock->Foreground = ref new SolidColorBrush(Windows::UI::Colors::Gold);
	textBlock->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
	textBlock->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
	
	Window::Current->Content = textBlock;
	Window::Current->Activate();
}

