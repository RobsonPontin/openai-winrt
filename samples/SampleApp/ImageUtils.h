#pragma once

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

namespace SampleApp::Utils
{
	class ImageUtils
	{
	public:
		IAsyncAction CreateMaskFromImage(Windows::Storage::StorageFile const& file, Windows::Foundation::Rect rect);
	};
}