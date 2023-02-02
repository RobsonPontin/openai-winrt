#pragma once

#include "BaseResponse.g.h"

namespace winrt::OpenAI::implementation
{
	struct BaseResponse : BaseResponseT< BaseResponse>
	{
		BaseResponse() {}

		virtual bool IsResponseSuccess()
		{
			return false;
		}
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct BaseResponse : BaseResponseT<BaseResponse, implementation::BaseResponse>
	{
	};
}