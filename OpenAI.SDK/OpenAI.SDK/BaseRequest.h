#pragma once

#include "BaseRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::implementation
{
	struct BaseRequest : BaseRequestT<BaseRequest>
	{
		BaseRequest() {}

		virtual bool IsValid()
		{
			return false;
		}

		virtual WWH::HttpRequestMessage BuildHttpRequest() { return nullptr; }
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct BaseRequest : BaseRequestT<BaseRequest, implementation::BaseRequest>
	{
	};
}