#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "winrt/Windows.Storage.Pickers.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.UI.Xaml.Media.Imaging.h"

using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;

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

	IAsyncOperation<Windows::Storage::Streams::IRandomAccessStream> ToStreamAsync(Windows::Storage::Streams::IBuffer ibuffer)
	{
		auto stream = InMemoryRandomAccessStream();
		auto outputStream = stream.GetOutputStreamAt(0);
		auto datawriter = DataWriter(outputStream);
		datawriter.WriteBuffer(ibuffer);
		co_await datawriter.StoreAsync();
		co_await outputStream.FlushAsync();
		co_return stream;
	}

    IAsyncAction MainPage::btnImageCreate_click(IInspectable const&, RoutedEventArgs const&)
    {
		m_openAiService.SetOpenAiSecretKey(L"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");

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
					auto stream = co_await ToStreamAsync(buffer);

					if (stream != nullptr)
					{
						auto img = BitmapImage{};
						img.SetSource(stream);
						image().Source(img);
					}
				}
			}
		}

		co_return;


    }
}
