#include "pch.h"

#include "ImageResponse.h"
#if __has_include("Image.ImageResponse.g.cpp")
#include "Image.ImageResponse.g.cpp"
#endif

namespace winrt::OpenAI::Image::implementation
{
	ImageResponse::ImageResponse(std::vector<WS::Streams::IBuffer> images)
	{
		m_images = images;
	}

	ImageResponse::ImageResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}
}