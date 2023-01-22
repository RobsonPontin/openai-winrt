#include "pch.h"

#include "EmbeddingResponse.h"
#if __has_include("Embedding.EmbeddingResponse.g.cpp")
#include "Embedding.EmbeddingResponse.g.cpp"
#endif

namespace winrt::OpenAI::Embedding::implementation
{
	EmbeddingResponse::EmbeddingResponse(hstring responseText)
	{
		m_responseText = responseText;
	}
}