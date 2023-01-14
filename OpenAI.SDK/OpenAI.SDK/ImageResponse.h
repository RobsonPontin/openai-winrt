#pragma once

#include "Image.ImageResponse.g.h"

namespace winrt::OpenAI::Image::implementation
{
	struct ImageResponse : ImageResponseT<ImageResponse>
	{
		ImageResponse() {}

		bool IsResponseSuccess()
		{
			return m_isResponseSuccess;
		}

	private:
		bool m_isResponseSuccess{ false };
	};
}

namespace winrt::OpenAI::Image::factory_implementation
{
	struct ImageResponse : ImageResponseT<ImageResponse, implementation::ImageResponse>
	{
	};
}
