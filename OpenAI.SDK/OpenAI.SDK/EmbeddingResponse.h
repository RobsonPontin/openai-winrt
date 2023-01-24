#pragma once

#include "Embedding.EmbeddingResponse.g.h"
#include "Embedding.EmbeddingValue.g.h"


namespace winrt::OpenAI::Embedding::implementation
{
	struct EmbeddingResponse : EmbeddingResponseT<EmbeddingResponse>
	{
		EmbeddingResponse() {}
		EmbeddingResponse(hstring responseText);

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

		WF::Collections::IVector<EmbeddingValue> Data()
		{
			return m_embedding;
		}

	private:
		hstring m_responseText{ L"" };
		WF::Collections::IVector<EmbeddingValue> m_embedding;
	};

	struct EmbeddingValue : EmbeddingValueT<EmbeddingValue>
	{
		EmbeddingValue() {}
		EmbeddingValue(int32_t index, std::vector<double> embedding);

		int32_t Index()
		{
			return m_index;
		}

		WF::Collections::IVector<double> Embedding()
		{
			return m_embedding;
		}


	private:
		int32_t m_index = 0;
		WF::Collections::IVector<double> m_embedding;
	};
}

namespace winrt::OpenAI::Embedding::factory_implementation
{
	struct EmbeddingResponse : EmbeddingResponseT<EmbeddingResponse, implementation::EmbeddingResponse>
	{
	};

	struct EmbeddingValue : EmbeddingValueT<EmbeddingValue, implementation::EmbeddingValue>
	{
	};
}