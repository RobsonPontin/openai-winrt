#pragma once

#include "Embedding.EmbeddingResponse.g.h"


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

	private:
		hstring m_responseText{ L"" };
	};
}

namespace winrt::OpenAI::Embedding::factory_implementation
{
	struct EmbeddingResponse : EmbeddingResponseT<EmbeddingResponse, implementation::EmbeddingResponse>
	{
	};
}