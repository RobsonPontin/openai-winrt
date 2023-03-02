#pragma once
#include "FilesRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::implementation
{
	struct FilesRequest : FilesRequestT<FilesRequest, BaseRequest>
	{
		FilesRequest() {}

		bool IsValid()
		{
			return true;
		}

		WWH::HttpRequestMessage BuildHttpRequest();
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct FilesRequest : FilesRequestT<FilesRequest, implementation::FilesRequest>
	{
	};
}