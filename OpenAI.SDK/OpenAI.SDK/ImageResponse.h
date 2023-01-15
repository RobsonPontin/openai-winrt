#pragma once

#include "Image.ImageResponse.g.h"

namespace winrt::OpenAI::Image::implementation
{
	struct ImageResponse : ImageResponseT<ImageResponse>
	{
		ImageResponse() {}
		ImageResponse(std::vector<WS::Streams::IBuffer> images);

		bool IsResponseSuccess()
		{
			if (Images().Size() > 0)
			{
				return true;
			}

			return false;
		}

		WF::Collections::IVector<WS::Streams::IBuffer> Images()
		{
			Windows::Foundation::Collections::IVector<WS::Streams::IBuffer> vector
			{ 
				winrt::single_threaded_vector<WS::Streams::IBuffer>() 
			};
			
			for (int i = 0; i < m_images.size(); ++i)
			{
				vector.Append(m_images[i]);
			}

			return vector;
		}

	private:
		bool m_isResponseSuccess{ false };
		std::vector<WS::Streams::IBuffer> m_images;
	};
}

namespace winrt::OpenAI::Image::factory_implementation
{
	struct ImageResponse : ImageResponseT<ImageResponse, implementation::ImageResponse>
	{
	};
}
