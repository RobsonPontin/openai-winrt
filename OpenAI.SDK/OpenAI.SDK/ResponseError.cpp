#include "pch.h"

#include "ResponseError.h"
#if __has_include("ResponseError.g.cpp")
#include "ResponseError.g.cpp"
#endif


namespace winrt::OpenAI::implementation
{
	ResponseError::ResponseError(winrt::hstring const& message)
	{
		m_errorMessage = message;
	}

	ResponseError::ResponseError(winrt::hstring const& code, winrt::hstring const& message, winrt::hstring const& type)
	{
		m_errorCode = code;
		m_errorMessage = message;
		m_errorType = type;
	}
}