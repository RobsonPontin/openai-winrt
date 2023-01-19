#pragma once

#include "Embedding.EmbeddingRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::Embedding::implementation
{
	struct EmbeddingRequest : EmbeddingRequestT<EmbeddingRequest>
	{
		EmbeddingRequest() {}

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

namespace winrt::OpenAI::Embedding::factory_implementation
{
	struct EmbeddingRequest : EmbeddingRequestT<EmbeddingRequest, implementation::EmbeddingRequest>
	{
	};
}