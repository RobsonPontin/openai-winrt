#pragma once

#include "Completion.CompletionRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::Completion::implementation
{
	struct CompletionRequest : CompletionRequestT<CompletionRequest>
	{
		CompletionRequest() {}

		OpenAI::ModelType Model()
		{
			return m_model;
		}
		void Model(OpenAI::ModelType val)
		{
			m_model = val;
		}
		
		winrt::hstring Prompt()
		{
			return m_prompt;
		}
		void Prompt(winrt::hstring val)
		{
			m_prompt = val;
		}

		bool IsValid()
		{
			if (Prompt() != L"")
			{
				return true;
			}

			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		winrt::hstring m_prompt{ L"" };
		OpenAI::ModelType m_model{ ModelType::text_davinci_003 };
	};
}

namespace winrt::OpenAI::Completion::factory_implementation
{
	struct CompletionRequest : CompletionRequestT<CompletionRequest, implementation::CompletionRequest>
	{
	};
}