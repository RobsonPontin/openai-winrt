#pragma once

#include "MainPage.g.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace winrt::SampleApp::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        IAsyncAction btnImageCreate_click(IInspectable const& sender, RoutedEventArgs const& args);

    private:
        OpenAI_SDK::OpenAiService m_openAiService;
    };
}

namespace winrt::SampleApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
