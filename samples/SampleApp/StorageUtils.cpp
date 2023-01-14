#include "pch.h"
#include "StorageUtils.h"

#include "winrt/Windows.Storage.Pickers.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.Graphics.Display.h"
#include "winrt/Windows.Graphics.Imaging.h"
#include "winrt/Windows.UI.Xaml.Media.Imaging.h"

using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Imaging;


namespace SampleApp::Utils
{
    IAsyncOperation<IRandomAccessStream> StorageUtils::ToStreamAsync(Windows::Storage::Streams::IBuffer ibuffer)
    {
        auto stream = InMemoryRandomAccessStream();
        auto outputStream = stream.GetOutputStreamAt(0);
        auto datawriter = DataWriter(outputStream);
        datawriter.WriteBuffer(ibuffer);
        co_await datawriter.StoreAsync();
        co_await outputStream.FlushAsync();
        co_return stream;
    }

    IAsyncOperation<StorageFile> StorageUtils::CreateFileFromImageAsync(Image image, bool saveToTempFolder)
    {
        auto display = DisplayInformation::GetForCurrentView();
        auto renderTargetBitmap = RenderTargetBitmap();
        co_await renderTargetBitmap.RenderAsync(image, (int)image.Width(), (int)image.Height());

        IBuffer pixels = co_await renderTargetBitmap.GetPixelsAsync();

        auto reader = ::Windows::Storage::Streams::DataReader::FromBuffer(pixels);
        std::vector<unsigned char> data(reader.UnconsumedBufferLength());
        if (!data.empty())
        {
            reader.ReadBytes(data);
        }

        StorageFile file{ nullptr };
        if (!saveToTempFolder)
        {
            auto savePicker = Pickers::FileSavePicker();
            savePicker.SuggestedStartLocation(Pickers::PickerLocationId::PicturesLibrary);
            savePicker.FileTypeChoices().Insert(L"PNG", winrt::single_threaded_vector<winrt::hstring>({ L".png" }));
            // Default file name if the user does not type one in or select a file to replace
            savePicker.SuggestedFileName(L"AI image");

            file = co_await savePicker.PickSaveFileAsync();
        }
        else
        {
            auto tempFolder = ApplicationData::Current().TemporaryFolder();


            file = co_await tempFolder.CreateFileAsync(L"ai_image", CreationCollisionOption::GenerateUniqueName);
        }

        if (file != nullptr)
        {
            auto stream = co_await file.OpenAsync(FileAccessMode::ReadWrite);
            auto encoder = co_await BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId(), stream);
            encoder.SetPixelData(
                BitmapPixelFormat::Bgra8, 
                BitmapAlphaMode::Ignore,
                static_cast<uint32_t>(image.ActualWidth()),
                static_cast<uint32_t>(image.ActualHeight()),
                display.LogicalDpi(),
                display.LogicalDpi(),
                data);

            co_await encoder.FlushAsync();
        }

        co_return file;
    }
}