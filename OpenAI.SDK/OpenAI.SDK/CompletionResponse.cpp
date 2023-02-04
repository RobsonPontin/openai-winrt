#include "pch.h"

#include "CompletionResponse.h"
#if __has_include("Completion.CompletionResponse.g.cpp")
#include "Completion.CompletionResponse.g.cpp"
#endif

namespace winrt::OpenAI::Completion::implementation
{
	CompletionResponse::CompletionResponse(hstring responseText)
	{
		m_responseText = responseText;
	}

	CompletionResponse::CompletionResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}
}