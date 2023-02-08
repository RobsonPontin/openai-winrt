#pragma once

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

namespace SampleApp::Utils
{
	class StorageUtils
	{
	public:
		static IAsyncOperation<IRandomAccessStream> ToStreamAsync(IBuffer ibuffer);
		static IAsyncOperation<StorageFile> CreateFileFromImageAsync(Image image, bool saveToTempFolder);
	};
}