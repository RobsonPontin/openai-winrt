#pragma once

#include "Embedding.EmbeddingResponse.g.h"
#include "Embedding.EmbeddingValue.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Embedding::implementation
{
	struct EmbeddingResponse : EmbeddingResponseT<EmbeddingResponse, OpenAI::implementation::BaseResponse>
	{
		EmbeddingResponse() {}
		EmbeddingResponse(WF::Collections::IVector<Embedding::EmbeddingValue> const& embedding);
		EmbeddingResponse(OpenAI::ResponseError const& error);

		bool IsResponseSuccess()
		{
			if (Data().Size() > 0)
			{
				return true;
			}

			return false;
		}

		WF::Collections::IVector<Embedding::EmbeddingValue> Data()
		{
			return m_embedding;
		}

	private:
		WF::Collections::IVector<Embedding::EmbeddingValue> m_embedding;
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
			auto result{ winrt::single_threaded_vector<double>(std::move(m_embedding)) };
			return result;
		}


	private:
		int32_t m_index = 0;
		std::vector<double> m_embedding;
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