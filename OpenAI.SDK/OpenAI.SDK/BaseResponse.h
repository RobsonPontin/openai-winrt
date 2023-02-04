#pragma once

#include "BaseResponse.g.h"

namespace winrt::OpenAI::implementation
{
	struct BaseResponse : BaseResponseT<BaseResponse>
	{
		BaseResponse() {}
		BaseResponse(OpenAI::ResponseError const& error);

		virtual bool IsResponseSuccess()
		{
			return false;
		}

	    virtual OpenAI::ResponseError Error()
		{
			return m_error;
		}

		OpenAI::ResponseError m_error{ nullptr };
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct BaseResponse : BaseResponseT<BaseResponse, implementation::BaseResponse>
	{
	};
}