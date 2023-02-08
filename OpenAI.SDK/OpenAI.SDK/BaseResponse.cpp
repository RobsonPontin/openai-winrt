#include "pch.h"

#include "BaseResponse.h"
#if __has_include("BaseResponse.g.cpp")
#include "BaseResponse.g.cpp"
#endif

namespace winrt::OpenAI::implementation
{
	BaseResponse::BaseResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}
}