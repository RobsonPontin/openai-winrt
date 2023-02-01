#include "pch.h"

#include "ResponseError.h"
#if __has_include("ResponseError.g.cpp")
#include "ResponseError.g.cpp"
#endif


namespace winrt::OpenAI::implementation
{
	ResponseError::ResponseError(winrt::hstring errorMessage)
	{
		m_errorMessage = errorMessage;
	}
}