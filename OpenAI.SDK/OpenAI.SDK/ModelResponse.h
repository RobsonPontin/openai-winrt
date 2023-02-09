#pragma once
#include "ModelResponse.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::implementation
{
	struct ModelResponse : ModelResponseT< ModelResponse, OpenAI::implementation::BaseResponse>
	{
		ModelResponse() {}
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct ModelResponse : ModelResponseT<ModelResponse, implementation::ModelResponse>
	{
	};
}
