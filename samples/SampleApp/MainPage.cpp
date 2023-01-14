#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "winrt/Windows.Storage.Pickers.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.UI.Xaml.Media.Imaging.h"

#include "StorageUtils.h"

using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Controls;


namespace winrt::SampleApp::implementation
{
    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

	void MainPage::MenuFlyoutItem_Click(IInspectable const& sender, RoutedEventArgs const&)
	{
		auto flyoutItem = sender.try_as<MenuFlyoutItem>();
		if (flyoutItem)
		{
			auto tag = winrt::unbox_value_or<hstring>(flyoutItem.Tag(), L"");
			if (tag == L"Variant")
			{
				tbImagePromt().IsEnabled(false);
				m_actionSelected = ImageActionType::Variant;
			}
			else if (tag == L"Create")
			{
				tbImagePromt().IsEnabled(true);
				m_actionSelected = ImageActionType::Create;
			}
			else if (tag == L"Edit")
			{
				tbImagePromt().IsEnabled(true);
				m_actionSelected = ImageActionType::Edit;
			}
		}
	}

	IAsyncAction MainPage::btnProcessImage_Click(IInspectable const& sender, RoutedEventArgs const& e)
	{
		if (tbOpenAiKey().Text() != L"")
		{
			m_openAiService.SetOpenAiSecretKey(tbOpenAiKey().Text());
		}
		else
		{
			return;
		}

		switch (m_actionSelected)
		{
		case ImageActionType::Create:

			if (tbImagePromt().Text() != L"")
			{
				co_await ProcessImageCreationAsync(tbImagePromt().Text());
			}
			break;

		case ImageActionType::Edit:
			break;

		case ImageActionType::Variant:
			co_await ProcessImageVariantAsync();
			break;
		}
	}

	IAsyncAction MainPage::btnSaveImage_Click(IInspectable const& sender, RoutedEventArgs const& e)
	{
		co_await ::SampleApp::Utils::StorageUtils::CreateFileFromImageAsync(image(), false);
	}

	IAsyncAction MainPage::ProcessImageVariantAsync()
	{
		if (!m_openAiService.IsRunning())
		{
			auto openPicker = Pickers::FileOpenPicker();
			openPicker.ViewMode(Pickers::PickerViewMode::Thumbnail);
			openPicker.SuggestedStartLocation(Pickers::PickerLocationId::PicturesLibrary);
			openPicker.FileTypeFilter().Append(L".png");

			auto file = co_await openPicker.PickSingleFileAsync();

			if (file != nullptr)
			{
				auto buffer = co_await m_openAiService.GenerateDalleVariantAsync(file);

				if (buffer != nullptr)
				{
					auto stream = co_await ::SampleApp::Utils::StorageUtils::ToStreamAsync(buffer);

					if (stream != nullptr)
					{
						auto img = BitmapImage{};
						img.SetSource(stream);
						image().Source(img);
					}
				}
			}
		}
	}

	IAsyncAction MainPage::ProcessImageCreationAsync(winrt::hstring prompt)
	{
		if (!m_openAiService.IsRunning())
		{
			auto buffer = co_await m_openAiService.GenerateDalleImageAsync(prompt);

			if (buffer != nullptr)
			{
				auto stream = co_await ::SampleApp::Utils::StorageUtils::ToStreamAsync(buffer);

				if (stream != nullptr)
				{
					auto img = BitmapImage{};
					img.SetSource(stream);
					image().Source(img);
				}
			}
		}
	}
}

