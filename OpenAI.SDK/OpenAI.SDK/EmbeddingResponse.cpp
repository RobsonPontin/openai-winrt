#include "pch.h"

#include "EmbeddingResponse.h"
#if __has_include("Embedding.EmbeddingResponse.g.cpp")
#include "Embedding.EmbeddingResponse.g.cpp"
#endif

#if __has_include("Embedding.EmbeddingValue.g.cpp")
#include "Embedding.EmbeddingValue.g.cpp"
#endif

namespace winrt::OpenAI::Embedding::implementation
{
	// Embedding Response

	EmbeddingResponse::EmbeddingResponse(WF::Collections::IVector<Embedding::EmbeddingValue> const& embedding)
	{
		m_embedding = embedding;
	}

	// Embedding Value

	EmbeddingValue::EmbeddingValue(int32_t index, std::vector<double> embedding)
	{
		m_index = index;
		m_embedding = embedding;
	}
}