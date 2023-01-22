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

		winrt::hstring Input()
		{
			return m_input;
		}
		void Input(winrt::hstring val)
		{
			m_input = val;
		}

		bool IsValid()
		{
			if (Input() != L"")
			{
				return true;
			}

			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		winrt::hstring m_input{ L"" };
		OpenAI::ModelType m_model{ ModelType::text_embedding_ada_002 };
	};
}

namespace winrt::OpenAI::Embedding::factory_implementation
{
	struct EmbeddingRequest : EmbeddingRequestT<EmbeddingRequest, implementation::EmbeddingRequest>
	{
	};
}