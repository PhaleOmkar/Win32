#include "pch.h"
#include "Hello.h"
#include "MyPage.h"


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

	MyPage^ page = ref new MyPage();
	Window::Current->Content = page;
	Window::Current->Activate();

}
