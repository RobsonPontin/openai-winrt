#pragma once

#include "OpenAiOptions.g.h"

namespace winrt::OpenAI::implementation
{
	struct OpenAiOptions : OpenAiOptionsT<OpenAiOptions>
	{
		OpenAiOptions() {}
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct OpenAiOptions : OpenAiOptionsT<OpenAiOptions, implementation::OpenAiOptions>
	{
	};
}

