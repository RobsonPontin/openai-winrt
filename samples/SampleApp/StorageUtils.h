#pragma once

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

namespace SampleApp::Utils
{
	class StorageUtils
	{
		static IAsyncOperation<StorageFile> CreateFileFromImageAsync(Streams::IBuffer buffer, bool saveToTempFolder);
	};
}