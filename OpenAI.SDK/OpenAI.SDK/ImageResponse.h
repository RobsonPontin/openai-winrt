#pragma once

#include "Image.ImageResponse.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Image::implementation
{
	struct ImageResponse : ImageResponseT<ImageResponse, OpenAI::implementation::BaseResponse>
	{
		ImageResponse() {}
		ImageResponse(std::vector<WSS::IBuffer> images);
		ImageResponse(OpenAI::ResponseError const& error);

		bool IsResponseSuccess()
		{
			if (Images().Size() > 0)
			{
				return true;
			}

			return false;
		}

		WF::Collections::IVector<WSS::IBuffer> Images()
		{
			Windows::Foundation::Collections::IVector<WSS::IBuffer> vector
			{ 
				winrt::single_threaded_vector<WSS::IBuffer>() 
			};
			
			for (int i = 0; i < m_images.size(); ++i)
			{
				vector.Append(m_images[i]);
			}

			return vector;
		}

	private:
		std::vector<WSS::IBuffer> m_images;
	};
}

namespace winrt::OpenAI::Image::factory_implementation
{
	struct ImageResponse : ImageResponseT<ImageResponse, implementation::ImageResponse>
	{
	};
}
