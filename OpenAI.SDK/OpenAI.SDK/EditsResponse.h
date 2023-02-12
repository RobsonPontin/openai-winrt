#pragma once

#include "Edits.EditsResponse.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Edits::implementation
{
	struct EditsResponse : EditsResponseT<EditsResponse, OpenAI::implementation::BaseResponse>
	{
		EditsResponse() {}
	    EditsResponse(OpenAI::ResponseError const& error);
	};
}

namespace winrt::OpenAI::Edits::factory_implementation
{
	struct EditsResponse : EditsResponseT<EditsResponse, implementation::EditsResponse>
	{
	};
}