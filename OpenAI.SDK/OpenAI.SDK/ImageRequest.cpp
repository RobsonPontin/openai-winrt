#include "pch.h"

#include "ImageRequest.h"
#if __has_include("Image.ImageRequest.g.cpp")
#include "Image.ImageRequest.g.cpp"
#endif

#if __has_include("Image.ImageCreateRequest.g.cpp")
#include "Image.ImageCreateRequest.g.cpp"
#endif

#if __has_include("Image.ImageVariationRequest.g.cpp")
#include "Image.ImageVariationRequest.g.cpp"
#endif

#if __has_include("Image.ImageEditRequest.g.cpp")
#include "Image.ImageEditRequest.g.cpp"
#endif

#include "ImageUtils.h"


namespace winrt::OpenAI::Image::implementation
{
	WF::IAsyncOperation<bool> ImageVariationRequest::SetImageAsync(WS::StorageFile file)
	{
		auto imgBuffer = co_await ::OpenAI::Utils::Convert::FileToPngBufferAsync(file);
		if (imgBuffer)
		{
			m_imageBuffer = imgBuffer;

			co_return true;
		}

		co_return false;
	}

	WF::IAsyncOperation<bool> ImageEditRequest::SetImageAsync(WS::StorageFile file)
	{
		auto imgBuffer = co_await ::OpenAI::Utils::Convert::FileToPngBufferAsync(file);
		if (imgBuffer)
		{
			m_imageBuffer = imgBuffer;

			co_return true;
		}

		co_return false;
	}
}