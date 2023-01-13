#include "pch.h"
#include "StorageUtils.h"

#include "winrt/Windows.Storage.Pickers.h"

namespace SampleApp::Utils
{
    IAsyncOperation<StorageFile> StorageUtils::CreateFileFromImageAsync(IBuffer buffer, bool saveToTempFolder)
    {
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
            co_await FileIO::WriteBufferAsync(file, buffer);
        }

        co_return file;
    }
}