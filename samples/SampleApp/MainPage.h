#pragma once

#include "MainPage.g.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace winrt::SampleApp::implementation
{
    enum ActionType
    {
        ImageCreate,
        ImageVariation,
        ImageEdit,
        TextCompletion
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
        ActionType m_actionSelected{ ActionType::ImageCreate };

        IAsyncAction ProcessImageCreationAsync(winrt::hstring prompt);
        IAsyncAction ProcessImageVariantAsync();
        IAsyncAction ProcessImageEditAsync(winrt::hstring prompt);

        IAsyncAction ProcessTextCompletionAsync(winrt::hstring prompt);

        void ShowTextResult();
        void ShowImageResult();
    };
}

namespace winrt::SampleApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
