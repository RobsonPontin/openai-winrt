#pragma once

#include "winrt/Windows.UI.Xaml.Media.Imaging.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;

namespace Utils
{
	class ImageUtils
	{
	public:
		static IAsyncOperation<WriteableBitmap> CreateMaskAsync(
			int32_t width,
			int32_t height, 
			Windows::Foundation::Rect rect);
	};
}