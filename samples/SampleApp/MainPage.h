#pragma once

#include "MainPage.g.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace winrt::SampleApp::implementation
{
    enum ImageActionType
    {
        Create,
        Variant,
        Edit
    };

    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

    public:
        IAsyncAction btnProcessImage_Click(IInspectable const& sender, RoutedEventArgs const& e);
        void MenuFlyoutItem_Click(IInspectable const& sender, RoutedEventArgs const& e);
        IAsyncAction btnSaveImage_Click(IInspectable const& sender, RoutedEventArgs const& e);

    private:
        OpenAI::OpenAiService m_openAiService;
        ImageActionType m_actionSelected{ ImageActionType::Create };

        IAsyncAction ProcessImageCreationAsync(winrt::hstring prompt);
        IAsyncAction ProcessImageVariantAsync();
        IAsyncAction ProcessImageEditAsync(winrt::hstring prompt);
    };
}

namespace winrt::SampleApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
