#pragma once
#include "FilesResponse.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::implementation
{
	struct FilesResponse : FilesResponseT< FilesResponse, OpenAI::implementation::BaseResponse>
	{
		FilesResponse() {}
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct FilesResponse : FilesResponseT<FilesResponse, implementation::FilesResponse>
	{
	};
}