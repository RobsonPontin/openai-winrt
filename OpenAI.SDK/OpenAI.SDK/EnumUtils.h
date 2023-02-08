#pragma once

#include "OpenAiService.h"


namespace Utils::Converters
{
	winrt::hstring ModelTypeToString(winrt::OpenAI::ModelType model);

	winrt::hstring ResponseFormatToString(winrt::OpenAI::Image::ResponseFormatType format);

	winrt::hstring ImageSizeToString(winrt::OpenAI::Image::SizeType size);
}