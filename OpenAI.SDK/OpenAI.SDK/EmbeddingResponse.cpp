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
	EmbeddingResponse::EmbeddingResponse(hstring responseText)
	{
		m_responseText = responseText;
	}

	EmbeddingValue::EmbeddingValue(int32_t index, std::vector<double> embedding)
	{
		m_index = index;
		// TODO: setup WF List from vector
		//m_embedding = embedding;
	}
}