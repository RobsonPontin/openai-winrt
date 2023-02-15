#include "pch.h"

#include "EditsResponse.h"
#if __has_include("Edits.EditsResponse.g.cpp")
#include "Edits.EditsResponse.g.cpp"
#endif

#if __has_include("Edits.EditsChoice.g.cpp")
#include "Edits.EditsChoice.g.cpp"
#endif

#if __has_include("Edits.EditsUsage.g.cpp")
#include "Edits.EditsUsage.g.cpp"
#endif

namespace winrt::OpenAI::Edits::implementation
{
	// Response

	EditsResponse::EditsResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}

	EditsResponse::EditsResponse(std::vector<Edits::EditsChoice> editsChoices, Edits::EditsUsage const& editsUsage)
	{
		m_editsChoices = editsChoices;
		m_editsUsage = editsUsage;
	}

	// Choice Value

	EditsChoice::EditsChoice(uint32_t index, winrt::hstring text)
	{
		m_index = index;
		m_text = text;
	}

	// Usage

	EditsUsage::EditsUsage(uint32_t promptTokens, uint32_t completionTokens, uint32_t totalTokens)
	{
		m_promptTokens = promptTokens;
		m_completionTokens = completionTokens;
		m_totalTokens = totalTokens;
	}
}