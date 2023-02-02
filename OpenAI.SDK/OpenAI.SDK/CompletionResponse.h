#pragma once

#include "Completion.CompletionResponse.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Completion::implementation
{
	struct CompletionResponse : CompletionResponseT<CompletionResponse, OpenAI::implementation::BaseResponse>
	{
		CompletionResponse() {}
		CompletionResponse(hstring responseText);

		bool IsResponseSuccess()
		{
			if (ResponseText() != L"")
			{
				return true;
			}

			return false;
		}

		hstring ResponseText()
		{
			return m_responseText;
		}

	private:
		hstring m_responseText{ L"" };
	};
}

namespace winrt::OpenAI::Completion::factory_implementation
{
	struct CompletionResponse : CompletionResponseT<CompletionResponse, implementation::CompletionResponse>
	{
	};
}